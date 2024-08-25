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
        string selectedPort = "";
        int receivedBytes = 0;
        string buffer = "";
        public Form1()
        {
            thisForm = this;
            InitializeComponent();


            Size = new Size(800, 700);
            panelWaiting.Dock = DockStyle.Fill;
            panelFlashing.Dock = DockStyle.Fill;
            setModeWaiting();
        }

        private SerialPort port = null;
        long lastReceivedTime = Environment.TickCount;
        int st = 0; 
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            while (sp.IsOpen && sp.BytesToRead > 0)
            {
                string inp = sp.ReadExisting();
                //char inbyte = (char)sp.ReadByte();
                buffer += inp;
                receivedBytes+= inp.Length;
                st++;
                if(st %3 == 0)
                    updateCounters();
                lastReceivedTime = Environment.TickCount;
                //append(""+inbyte);
                //log("Byte received: " + inbyte);
                if (inp.Contains("};"))
                    log("Done.");
                    //disconnect();

            }
        }
        private void buttonFlash_Click(object sender, EventArgs e)
        {
            richTextBoxLog.Clear();
            buffer = "";

            if (selectedPort.Equals(""))
            {
                log("Не обрано COM порт");
                return;
            }
            buttonFlashEnable(false);
            new Thread(() =>
            {
                try
                {
                    if (port != null)
                    {
                        log("Вже підключено");
                        return;
                    }
                    log("Connecting to port: " + selectedPort + "...");
                    port = new SerialPort(selectedPort, 115200);
                    port.DataReceived += DataReceivedHandler;
                    port.DtrEnable = true;
                    //port.RtsEnable = true;
                    port.Open();
                    log("Connected: " + selectedPort);
                    Thread.Sleep(1000);
                    send("<screenshot>");
                    //log("Отримую дані...");
                    //lastReceivedTime = Environment.TickCount;
                    //while (Environment.TickCount - lastReceivedTime < 4000) ;
                    //log("Дані більше не надходять.");
                    //updateCounters();
                    //disconnect();
                }
                catch(Exception ex)
                {
                    log("Помилка: "+ex.ToString());
                    port = null;
                }
                finally
                {
                    //port = null;
                    buttonFlashEnable(true);
                }
            }).Start();


        }


        public void disconnect()
        {
            log("Disconnecting: " + selectedPort);
            if (port != null)
            {
                try { port.DiscardInBuffer(); } catch (Exception) { };
                try { port.Dispose(); } catch (Exception) { };
                try { port.DiscardInBuffer(); } catch (Exception) { };
                port = null;
            }
            log("Disconnected.");
        }
        public bool send(string text)
        {
            if (port != null && port.IsOpen)
            {
                log("|#| <-- " + text);
                port.Write(text);
                return true;
            }
            else
            {
                log("|X| <-- " + text);
                return false;
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
        void updateCounters()
        {
            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { updateCounters(); }));
                return;
            }
            labelReceived.Text = "Отримано :" + receivedBytes + " байт";
            return;
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
                richTextBoxLog.Clear();
                buttonFlashEnable(true);
            }
            catch (Exception ex)
            {
                Console.WriteLine("setModeFlashing: " + ex.ToString());
            }
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
                        if (DeviceID.Contains("VID_303A&PID_80C2"))  //normal mode
                        //if (DeviceID.Contains("VID_303A&PID_0002"))  //boot mode
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


    }
}
