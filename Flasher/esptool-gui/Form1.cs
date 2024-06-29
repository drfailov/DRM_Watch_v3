using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;

namespace esptool_gui
{
    public partial class Form1 : Form
    {
        Form1 thisForm;
        bool enablePortsRefreshing = true;
        Process prc;
        string selectedPort = "";
        bool success = false;
        string flashingLog = "";
        public Form1()
        {
            thisForm = this;
            InitializeComponent();

            var MyIni = new IniFile("Settings.ini");
            textBoxFirmware.Text = MyIni.Read("textBoxFirmware");
            textBoxBootApp.Text = MyIni.Read("textBoxBootApp");
            textBoxBootloader.Text = MyIni.Read("textBoxBootloader");
            textBoxPartitions.Text = MyIni.Read("textBoxPartitions");

            Size = new Size(800, 700);
            panelWaiting.Dock = DockStyle.Fill;
            panelFlashing.Dock = DockStyle.Fill;
            panelSuccess.Dock = DockStyle.Fill;
            setModeWaiting();
        }

        private void buttonFlash_Click(object sender, EventArgs e)
        {
            richTextBoxLog.Clear();
            flashingLog = "";
            string firmware = textBoxFirmware.Text.Trim();
            string bootloader = textBoxBootloader.Text.Trim();
            string bootApp = textBoxBootApp.Text.Trim();
            string partitions = textBoxPartitions.Text.Trim();

            if (selectedPort.Equals(""))
            {
                log("Не обрано COM порт");
                return;
            }
            if (firmware.Equals(""))
            {
                log("Треба обрати файл Firmware");
                return;
            }
            if (!File.Exists(firmware))
            {
                log("Файл Firmware \"" + firmware + "\" не знайдено.");
                return;
            }
            if (partitions.Equals(""))
            {
                log("Треба обрати файл partitions");
                return;
            }
            if (!File.Exists(partitions))
            {
                log("Файл partitions \"" + partitions + "\" не знайдено.");
                return;
            }
            if (bootloader.Equals(""))
            {
                log("Треба обрати файл bootloader");
                return;
            }
            if (!File.Exists(bootloader))
            {
                log("Файл bootloader \"" + bootloader + "\" не знайдено.");
                return;
            }
            if (bootApp.Equals(""))
            {
                log("Треба обрати файл bootApp");
                return;
            }
            if (!File.Exists(bootApp))
            {
                log("Файл bootApp \"" + bootApp + "\" не знайдено.");
                return;
            }

            success = false;
            buttonFlashEnable(false);
            new Thread(() =>
            {
                try
                {
                    //esptool.exe --chip esp32s2 --port "COM5" --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 "bootloader.bin" 0x8000 "partitions.bin" 0xe000 "boot_app0.bin" 0x10000 "firmware.bin"
                    string args = "--chip esp32s2 --port \"" + selectedPort + "\" --baud 921600 write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 \""+ bootloader + "\" 0x8000 \"" + partitions + "\" 0xe000 \"" + bootApp + "\" 0x10000 \"" + firmware + "\" ";
                    log(args);
                    string output = RunCommandForResult(".\\", "esptool.exe", args);
                    //check if success
                    //success = true;
                    //regexp: (Wrote [0-9]+ bytes .+ seconds)

                    String report = "";
                    MatchCollection m = Regex.Matches(flashingLog, "(Wrote [0-9]+ bytes \\([0-9]+ compressed\\) at 0x[0-9abcdefABCDEF]+ in [0-9.]+ seconds)");
                    if (m.Count == 4) 
                    {
                        for (int i = 0; i < m.Count; i++)
                            report += "Файл " + (i+1) + ": " + m[i].Groups[0].Value + ";\n";
                        success = true;
                    }

                    if (success)
                        setModeSuccess(report.Trim('\n').Trim());
                    else
                    {
                        log("===========================");
                        log("Під час прошивки можливо виникли проблеми. Прогляньте лог і, якщо потрібно, спробуйте знову.");
                        log("===========================");
                    }
                }
                catch(Exception ex)
                {
                    log("Помилка: "+ex.ToString());
                }
            }).Start();


        }

        void p_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            flashingLog += e.Data;
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
                log(Path.GetFileName(filename) + " закритий.");
                Directory.SetCurrentDirectory(oldWorkingDir);
                buttonFlashEnable(true);
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

            catch (Exception ex)
            {
                Console.WriteLine("log: " + ex.ToString());
            }
            return text;
        }

        string status(string text)
        {

            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { status(text); }));
                return text;
            }
            try
            {
                labelStatus.Text = DateTime.UtcNow.ToString("HH:mm:ss")+" "+text;
            }
            catch (Exception ex)
            {
                Console.WriteLine("status: " + ex.ToString());
            }
            return text;
        }

        void buttonFlashEnable(bool val)
        {

            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { buttonFlashEnable(val); }));
                return;
            }
            try
            {
                textBoxFirmware.Enabled = val;
                textBoxPartitions.Enabled = val;
                textBoxBootloader.Enabled = val;
                textBoxBootApp.Enabled = val;
                buttonBrowseBootApp.Enabled = val;
                buttonBrowseBootloader.Enabled = val;
                buttonBrowseFirmware.Enabled = val;
                buttonBrowsePartitions.Enabled = val;
                buttonFlash.Enabled = val;
            }
            catch (Exception ex)
            {
                Console.WriteLine("status: " + ex.ToString());
            }
        }

        private void setModeWaiting()
        {
            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { setModeWaiting(); }));
                return;
            }
            try
            {
                panelWaiting.Visible = true;
                panelFlashing.Visible = false;
                panelSuccess.Visible = false;
                buttonFlashEnable(false);
            }
            catch (Exception ex)
            {
                Console.WriteLine("setModeWaiting: " + ex.ToString());
            }
        }
        private void setModeFlashing()
        {
            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { setModeFlashing(); }));
                return;
            }
            try
            {
                panelWaiting.Visible = false;
                panelFlashing.Visible = true;
                panelSuccess.Visible = false;
                richTextBoxLog.Clear();
                buttonFlashEnable(true);
            }
            catch (Exception ex)
            {
                Console.WriteLine("setModeFlashing: " + ex.ToString());
            }
        }
        private void setModeSuccess(String report)
        {
            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { setModeSuccess(report); }));
                return;
            }
            try
            {
                panelWaiting.Visible = false;
                panelFlashing.Visible = false;
                panelSuccess.Visible = true;
                labelReport.Text = report;
                buttonFlashEnable(true);
            }
            catch (Exception ex)
            {
                Console.WriteLine("setModeSuccess: " + ex.ToString());
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

        void refreshPorts()
        {
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM Win32_PnPEntity WHERE Caption like '%(COM%'"))
            {
                List<ManagementBaseObject> collection = searcher.Get().Cast<ManagementBaseObject>().ToList();
                if (!selectedPort.Equals(""))
                {
                    bool stillConnected = false;
                    foreach (ManagementBaseObject obj in collection)
                    {
                        string Caption = obj.GetPropertyValue("Caption").ToString();
                        if(Caption.Contains(selectedPort))
                            stillConnected = true;
                    }
                    if (stillConnected)
                        status("Підключено годинник в режимі прошивки: " + selectedPort);
                    else
                    {
                        // =========================================================================== DISCONNECTED ACTION
                        selectedPort = "";
                        setModeWaiting();
                    }
                }
                if (selectedPort.Equals(""))   //NO SELECTED PORT
                {
                    Console.WriteLine("=====");
                    foreach (ManagementBaseObject obj in collection)
                    {
                        string Caption = obj.GetPropertyValue("Caption").ToString();
                        string DeviceID = obj.GetPropertyValue("DeviceID").ToString();
                        Console.WriteLine("Caption=" + Caption);
                        Console.WriteLine("DeviceID=" + DeviceID);
                        //if (DeviceID.Contains("VID_303A&PID_80C2"))  //normal mode
                        if (DeviceID.Contains("VID_303A&PID_0002"))  //boot mode
                        {
                            // =========================================================================== CONNECTED ACTION
                            //todo extract COM port name
                            //Устройство с последовательным интерфейсом USB (COM4)
                            //find for: COM4
                            //regexp is: (COM[0-9]+)
                            Match m = Regex.Match(Caption, "(COM[0-9]+)");
                            if (m.Success)
                            {
                                Console.WriteLine("Found '" + m.Value + "' at position " + m.Index + ".");
                                selectedPort = m.Value;
                                setModeFlashing();
                            }
                        }
                    }
                    if (collection.Count == 0)
                        status("Не знайдено ніяких COM портів.");
                    else
                        status("Знайдено " + collection.Count + " COM портів, але годинник в режимі прошивки не знайдено.");
                }
            }
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
                        status("Помилка оновлення списку");
                        log("Помилка оновлення списку портів: " + ex.Message);
                    }
                    Thread.Sleep(500);
                }
                
            }).Start();
        }


        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            dlg.Title = "Оберіть файл Firmware";
            dlg.Filter = "Файл прошивки (*.bin)|*.bin";
            dlg.InitialDirectory = Environment.CurrentDirectory;
            dlg.Multiselect = false;
            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            foreach (var path in dlg.FileNames)
            {
                string relative = path;
                relative = MakeRelativePath(Environment.CurrentDirectory, relative);
                textBoxFirmware.Text = relative;
            }
        }
        private void buttonBrowsePartitions_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            dlg.Title = "Оберіть файл Partitions";
            dlg.Filter = "Файл прошивки (*.bin)|*.bin";
            dlg.InitialDirectory = Environment.CurrentDirectory;
            dlg.Multiselect = false;
            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            foreach (var path in dlg.FileNames)
            {
                string relative = path;
                relative = MakeRelativePath(Environment.CurrentDirectory, relative);
                textBoxPartitions.Text = relative;
            }
        }

        private void buttonBrowseBootloader_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            dlg.Title = "Оберіть файл Bootloader";
            dlg.Filter = "Файл прошивки (*.bin)|*.bin";
            dlg.InitialDirectory = Environment.CurrentDirectory;
            dlg.Multiselect = false;
            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            foreach (var path in dlg.FileNames)
            {
                string relative = path;
                relative = MakeRelativePath(Environment.CurrentDirectory, relative);
                textBoxBootloader.Text = relative;
            }
        }

        private void buttonBrowseBootApp_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            dlg.Title = "Оберіть файл BootApp";
            dlg.Filter = "Файл прошивки (*.bin)|*.bin";
            dlg.InitialDirectory = Environment.CurrentDirectory;
            dlg.Multiselect = false;
            if (dlg.ShowDialog() != DialogResult.OK)
                return;

            foreach (var path in dlg.FileNames)
            {
                string relative = path;
                relative = MakeRelativePath(Environment.CurrentDirectory, relative);
                textBoxBootApp.Text = relative;
            }
        }


        private void textBoxFirmware_TextChanged(object sender, EventArgs e)
        {
            var MyIni = new IniFile("Settings.ini");
            MyIni.Write("textBoxFirmware", textBoxFirmware.Text);
        }
        private void textBoxPartitions_TextChanged(object sender, EventArgs e)
        {
            var MyIni = new IniFile("Settings.ini");
            MyIni.Write("textBoxPartitions", textBoxPartitions.Text);
        }

        private void textBoxBootloader_TextChanged(object sender, EventArgs e)
        {
            var MyIni = new IniFile("Settings.ini");
            MyIni.Write("textBoxBootloader", textBoxBootloader.Text);
        }

        private void textBoxBootApp_TextChanged(object sender, EventArgs e)
        {
            var MyIni = new IniFile("Settings.ini");
            MyIni.Write("textBoxBootApp", textBoxBootApp.Text);
        }

        private void buttonGoBack_Click(object sender, EventArgs e)
        {
            setModeWaiting();
        }
    }
}
