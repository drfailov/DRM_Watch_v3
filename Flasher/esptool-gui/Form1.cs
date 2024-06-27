using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace esptool_gui
{
    public partial class Form1 : Form
    {
        Form1 thisForm;
        bool enablePortsRefreshing = true;
        Process prc;
        public Form1()
        {
            thisForm = this;
            InitializeComponent();

            var MyIni = new IniFile("Settings.ini");
            string path = MyIni.Read("binFile");
            textBoxFilePath.Text = path;

            Size = new Size(800, 600);
            panelLog.Visible = false;
            panelWaiting.Dock = DockStyle.Fill;
        }

        private void buttonFlash_Click(object sender, EventArgs e)
        {
            richTextBoxLog.Clear();
            string file = textBoxFilePath.Text;
            if (file.Trim().Equals(""))
            {
                log("Треба обрати файл");
                return;
            }
            string selectedPort = "";
            if(comboBoxCom.SelectedItem != null)
                selectedPort = comboBoxCom.SelectedItem.ToString();
            if (selectedPort.Trim().Equals(""))
            {
                log("Пошук порта...");
                buttonRefreshCom_Click(sender, e);
                if (comboBoxCom.SelectedItem != null)
                    selectedPort = comboBoxCom.SelectedItem.ToString();
                if (selectedPort.Trim().Equals(""))
                {
                    log("Підключіть ESP8266 для прошивки.");
                    return;
                }
            }
            string port ="";
            try
            {
                port = selectedPort.Split('-')[0].Trim();
                log("Порт для підключення: " + port);
            }
            catch (Exception ex)
            {
                log("Обрано невірний порт");
            }
            new Thread(() =>
            {
                if(!port.Equals("") && File.Exists(file))
                {
                    string args = "--chip esp8266 --port \"" + port + "\" --baud \"921600\" --before default_reset --after hard_reset write_flash 0x0 \"" + file + "\"";
                    log(args);
                    string output = RunCommandForResult(".\\", "esptool.exe", args);
                }
            }).Start();


        }

        void p_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            log(e.Data);
        }

        string RunCommandForResult(string path, string filename, string arguments, int timeout_s = 200)   //async!
        {
            //closeProcessIfRunnng();
            if (!File.Exists(path + filename))
                throw new Exception("Файла " + path + filename + " немає.");

            string oldWorkingDir = Directory.GetCurrentDirectory();
            Directory.SetCurrentDirectory(path);

            prc = new Process
            {
                StartInfo = new ProcessStartInfo(filename)
                {
                    Arguments = arguments,
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true

                }
            };
            prc.OutputDataReceived += p_OutputDataReceived;
            prc.ErrorDataReceived += p_OutputDataReceived;
            try
            {
                log("Запуск " + Path.GetFileName(filename) + " " + arguments + "...");
                prc.Start();
                prc.BeginOutputReadLine();
                prc.BeginErrorReadLine();
                prc.WaitForExit();
                //new Thread(() => processWatchdog(timeout_s)).Start();
                //string line = "";
                //string cline = "";
                //while (prc != null && !prc.HasExited)
                //{
                //    while (prc.StandardOutput.Peek() != -1)
                //    {
                //        char c = (char)prc.StandardOutput.Read();
                //        append(""+c);
                //        //if (c == '\n')
                //        //{
                //        //    log(cline);
                //        //    cline = "";
                //        //}
                //        //else
                //        //{
                //        //    cline += c;
                //        //}
                //    }

                //    while (prc.StandardError.Peek() != -1)
                //    {

                //        char c = (char)prc.StandardError.Read();
                //        append("" + c);
                //        //if (c == '\n')
                //        //{
                //        //    log(cline);
                //        //    cline = "";
                //        //}
                //        //else
                //        //{
                //        //    cline += c;
                //        //}
                //    }

                //    Thread.Sleep(100);

                //}
                //if (processTimeout)
                //{
                //    throw new Exception(log("Таймаут процесу! Завершую процес."));
                //}

                //if (prc != null)
                //{
                //    line += prc.StandardOutput.ReadToEnd();
                //    //log(line);
                //    line += "\n";
                //    line += prc.StandardError.ReadToEnd();
                //    //log(line);
                //}
                log(Path.GetFileName(filename) + " закритий.");
                Directory.SetCurrentDirectory(oldWorkingDir);
                return "";
            }
            finally
            {
                Directory.SetCurrentDirectory(oldWorkingDir);
            }
        }

        string append(string text)
        {
            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { append(text); }));
                return text;
            }
            richTextBoxLog.Text += text;
            //richTextBoxLog.SelectionStart = richTextBoxLog.Text.Length;
            //richTextBoxLog.ScrollToCaret();
            return text;
        }
        string log(string text)
        {

            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { log(text); }));
                return text;
            }
            try
            {
                richTextBoxLog.AppendText("\n" + text);
            }
            catch(Exception ex) { }
            
            //richTextBoxLog.SelectionStart = richTextBoxLog.Text.Length;
            //richTextBoxLog.ScrollToCaret();

            return text;
        }
        void deviceListClear()
        {

            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { deviceListClear(); }));
                return;
            }
            comboBoxCom.Items.Clear();
        }


        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            dlg.Filter = "Binary (*.bin)|*.bin";
            dlg.InitialDirectory = Environment.CurrentDirectory;
            dlg.Multiselect = false;
            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            foreach (var path in dlg.FileNames)
            {
                string relative = path;
                relative = MakeRelativePath(Environment.CurrentDirectory, relative);
                textBoxFilePath.Text = relative;
                var MyIni = new IniFile("Settings.ini");
                MyIni.Write("binFile", relative);
            }

        }

        public string MakeRelativePath(string workingDirectory, string fullPath)
        {
            string result = string.Empty;
            int offset;

            // this is the easy case.  The file is inside of the working directory.
            if (fullPath.StartsWith(workingDirectory))
            {
                return fullPath.Substring(workingDirectory.Length + 1);
            }

            // the hard case has to back out of the working directory
            string[] baseDirs = workingDirectory.Split(new char[] { ':', '\\', '/' });
            string[] fileDirs = fullPath.Split(new char[] { ':', '\\', '/' });

            // if we failed to split (empty strings?) or the drive letter does not match
            if (baseDirs.Length <= 0 || fileDirs.Length <= 0 || baseDirs[0] != fileDirs[0])
            {
                // can't create a relative path between separate harddrives/partitions.
                return fullPath;
            }

            // skip all leading directories that match
            for (offset = 1; offset < baseDirs.Length; offset++)
            {
                if (baseDirs[offset] != fileDirs[offset])
                    break;
            }

            // back out of the working directory
            for (int i = 0; i < (baseDirs.Length - offset); i++)
            {
                result += "..\\";
            }

            // step into the file path
            for (int i = offset; i < fileDirs.Length - 1; i++)
            {
                result += fileDirs[i] + "\\";
            }

            // append the file
            result += fileDirs[fileDirs.Length - 1];

            return result;
        }

        private void buttonRefreshCom_Click(object sender, EventArgs e)
        {

        }

        void refreshPorts()
        {
            deviceListClear();
            List<string> COMs = new List<string>();
            log("Get ports list...");
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM Win32_PnPEntity WHERE Caption like '%(COM%'"))
            {

                string[] portnames = SerialPort.GetPortNames();
                IEnumerable<string> ports = searcher.Get().Cast<ManagementBaseObject>().ToList().Select(p => p["Caption"].ToString());
                IEnumerable<string> ids = searcher.Get().Cast<ManagementBaseObject>().ToList().Select(p => p["DeviceID"].ToString());
                log("Names:");
                foreach (string port in portnames)
                    log(port);
                log("Descriptions:");
                foreach (string port in ports)
                    log(port);
                log("IDs:");
                foreach (string port in ids)
                    log(port);


                //

                List<string> portList = portnames.Select(n => n + " - " + ids.FirstOrDefault(s => s.Contains(n)) + "    " + ports.FirstOrDefault(s => s.Contains(n))).ToList();

                foreach (string s in portList)
                {
                    Console.WriteLine(s);
                    COMs.Add(s);
                }
            }

            int sel = 0;
            for (int i = 0; i < COMs.Count; i++)
            {
                string com = COMs[i];
                if (com.Contains("CH340"))
                    sel = i;
                comboBoxCom.Items.Add(com);
            }
            if (comboBoxCom.Items.Count > 0)
                comboBoxCom.SelectedIndex = sel;
            comboBoxCom.Enabled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            new Thread(() =>
            {
                while (thisForm.Visible)
                {
                    try
                    {
                        if (enablePortsRefreshing)
                        {
                            refreshPorts();
                        }
                    }
                    catch (Exception ex)
                    {
                        log("Помилка оновлення списку портів: " + ex.Message);
                    }
                    Thread.Sleep(500);
                }
                
            }).Start();
        }

        private void buttonLog_Click(object sender, EventArgs e)
        {
            panelLog.Visible = !panelLog.Visible;
        }
    }
}
