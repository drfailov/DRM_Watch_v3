using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Globalization;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace esptool_gui
{
    public partial class Form1 : Form
    {
        Form1 thisForm;
        bool enablePortsRefreshing = true;
        string selectedPort = "";
        int receivedBytes = 0;
        string buffer = "";
        bool endReceiving = false;
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
                buffer += inp;
                //parseString(inp);
                receivedBytes+= inp.Length;
                st++;
                if(st %3 == 0)
                    updateCounters();
                lastReceivedTime = Environment.TickCount;
                if (inp.Contains("};"))
                {
                    log("Отримано.");
                    endReceiving = true;
                }
            }
        }
        private void buttonFlash_Click(object sender, EventArgs e)
        {
            receivedBytes = 0;
            updateCounters();
            richTextBoxLog.Clear();
            buffer = "";
            endReceiving = false;

            if (selectedPort.Equals(""))
            {
                log("Не обрано COM порт");
                return;
            }
            buttonFlashEnable(false);
            buttonSaveEnable(false);
            pictureBox3.BackgroundImage = null;
            new Thread(() =>
            {
                try
                {
                    if (port != null)
                    {
                        log("Вже підключено");
                        return;
                    }
                    log("Підключення до " + selectedPort + "...");
                    port = new SerialPort(selectedPort, 115200, Parity.None, 8, StopBits.One);  //230400=8s  115200=8s  9600=8s
                    port.ErrorReceived += Port_ErrorReceived;
                    port.DataReceived += DataReceivedHandler;
                    port.WriteTimeout = 10;
                    port.ReadTimeout = 10;
                    port.Open();
                    
                    log("Підключено: " + selectedPort);
                    log("Відкрито: " + port.IsOpen);
                    port.DtrEnable = true;
                    Thread.Sleep(1000);

                    receivedBytes = 0;
                    buffer = "";
                    bool bufferParsed = false;
                    for (int i=0; i<3 && receivedBytes < 55000; i++)
                    {
                        receivedBytes = 0;
                        buffer = "";
                        log("Запит скріншоту...");
                        port.Write("<screenshot>");

                        log("Отримую дані...");
                        beginParse();
                        lastReceivedTime = Environment.TickCount;
                        int parsed = 0;
                        while (true)
                        {
                            int bufferLen = buffer.Length;
                            if (parsed < bufferLen)
                            {
                                parseString(buffer.Substring(parsed, bufferLen-parsed));
                                parsed = bufferLen;
                            }
                            bufferParsed = parsed == buffer.Length;
                            if (Environment.TickCount - lastReceivedTime > 4000 && bufferParsed)
                                break;
                            if (endReceiving && bufferParsed)
                                break;
                        }
                        
                        
                    }
                    updateCounters();
                    buttonSaveEnable(endReceiving && bufferParsed);

                    port.DtrEnable = false;
                    log("Відключення: " + selectedPort);
                    try { port.DiscardInBuffer(); } catch (Exception) { };
                    try { port.Close(); } catch (Exception) { };
                    try { port.Dispose(); } catch (Exception) { };
                    log("Відключено.");

                    //if (endReceiving)
                    //{
                    //    log("Парсінг...");
                    //    beginParse();
                    //    for(int i = 0;i<buffer.Length;i+=100)
                    //    {
                    //        log("Парсінг "+i+"...");
                    //        parseString(buffer.Substring(i, Math.Min(buffer.Length-i,100)));
                    //    }
                        
                    //}
                }
                catch(Exception ex)
                {
                    log("Помилка: "+ex.ToString());
                    port = null;
                }
                finally
                {
                    port = null;
                    buttonFlashEnable(true);
                }
            }).Start();
        }

        private void Port_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            log("Err: "+e.ToString());
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
            //labelReceived.Text = "Отримано: " + receivedBytes + " байт";

            //labelParsedWidth.Text = "Ширина: " + parseWidth + "px";
            //labelParsedHeight.Text = "Висота: " + parseHeight + "px";

            //labelX.Text = "X: " +  parseX+ "px";
            //labelY.Text = "Y: " +  parseY+ "px";
            pictureBox3.Refresh();
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
        void buttonSaveEnable(bool val)
        {

            if (InvokeRequired)
            {
                Invoke(new MethodInvoker(() => { buttonSaveEnable(val); }));
                return;
            }
            try
            {
                buttonSave.Enabled = val;
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
                        status("Підключено годинник: " + selectedPort);
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
                        status("Знайдено " + collection.Count + " COM портів, але годинник не знайдено.");
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


        Bitmap sourceBitmap = null;
        int parseWidth = 0;
        int parseHeight = 0;
        int parseX = 0;
        int parseY = 0;
        string parseBuffer = "";
        bool parseWaitWidth = false;
        bool parseWaitHeight = false;
        private void beginParse()
        {
            sourceBitmap = null;
            parseWidth = 0;
            parseHeight = 0;
            parseX = 0;
            parseY = 0;
            parseWaitWidth = false;
            parseWaitHeight = false;
        }
        private void parseString(string str)
        {
            for(int i=0; i<str.Length; i++)
            {
                char c = str[i];
                if(c == ' ' || c == '\n' || c == '{' || c == '}' || c == '\r' || c == ',')
                { //process buffer
                    if (parseBuffer.Equals("xbm_width"))
                        parseWaitWidth = true;
                    else if (parseBuffer.Equals("xbm_height"))
                        parseWaitHeight = true;
                    else if (parseWaitWidth)
                    {
                        if (Int32.TryParse(parseBuffer, out parseWidth))
                            parseWaitWidth = false;
                    }
                    else if (parseWaitHeight)
                    {
                        if (Int32.TryParse(parseBuffer, out parseHeight))
                            parseWaitHeight = false;
                    }
                    else if (parseBuffer.Trim().StartsWith("0x", StringComparison.CurrentCultureIgnoreCase)){
                        byte pixel;
                        bool parsedSuccessfully = byte.TryParse(parseBuffer.Trim().Substring(2), NumberStyles.HexNumber, CultureInfo.CurrentCulture, out pixel);
                        if(parsedSuccessfully && sourceBitmap != null)
                        {
                            lock (sourceBitmap)
                            {
                                for (int b = 0; b < 8; b++)
                                {
                                    int numberRightposition = pixel >> b;
                                    int bit = numberRightposition & 1;
                                    if (parseX + b < parseWidth && parseY < parseHeight)
                                        sourceBitmap.SetPixel(parseX + b, parseY, bit == 1 ? Color.White : Color.Black);
                                }
                            }
                            parseX += 8;
                        }
                    }
                    if(c == '\n' && !(parseY == 0 && parseX == 0))
                    {
                        parseY += 1;
                        parseX = 0;
                    }
                    if (sourceBitmap == null && parseWidth > 20 && parseHeight > 20)
                    {
                        sourceBitmap = new Bitmap(parseWidth, parseHeight);
                    }
                    parseBuffer = "";
                }
                else
                {
                    parseBuffer += c;
                }
            }
            updateCounters();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            //if (sourceBitmap != null)
            //{
            //    Graphics gForm = e.Graphics;
            //    gForm.DrawImage(sourceBitmap, 0, 0, sourceBitmap.Width, sourceBitmap.Height);
            //}
        }

        private void pictureBox3_Paint(object sender, PaintEventArgs e)
        {

            if (sourceBitmap != null)
            {
                Graphics gForm = e.Graphics;
                Font font = new Font(FontFamily.GenericMonospace, 8);
                Brush brush = new SolidBrush(Color.Black);
                int x = 0;
                int y = 0;
                int w = 0;
                int h = 0;
                lock (sourceBitmap)
                {
                    w = sourceBitmap.Width;
                    h = sourceBitmap.Height;
                    x = pictureBox3.Location.X + (pictureBox3.Size.Width-w)/2;
                    y = pictureBox3.Location.Y + (pictureBox3.Size.Height-h)/2;
                    gForm.DrawImage(sourceBitmap, x, y, w, h);
                }
                int textX = x + w + 5;
                int liheH = 15;
                int textY = y;

                gForm.DrawString("Отримано: " + receivedBytes + " байт", font, brush, textX, textY+=liheH);
                gForm.DrawString("Ширина: " + parseWidth + "px", font, brush, textX, textY += liheH);
                gForm.DrawString("Висота: " + parseHeight + "px", font, brush, textX, textY += liheH);
                gForm.DrawString("X: " + parseX + "px", font, brush, textX, textY += liheH);
                gForm.DrawString("Y: " + parseY + "px", font, brush, textX, textY += liheH);
            }
        }

        private void panelFlashing_Paint(object sender, PaintEventArgs e)
        {

        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.FileName = "DRM_Watch3_Screenshot_" + DateTime.Now.ToString("yyyyMMdd-HHmmss") + ".png";
            saveFileDialog1.Filter = "PNG files (*.png)|*.png";
            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                log("Збереження в файл...");
                sourceBitmap.Save(saveFileDialog1.FileName, ImageFormat.Png); // ImageFormat.Jpeg, etc
                log("Готово.");
            }
        }
    }
}
