namespace esptool_gui
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.richTextBoxLog = new System.Windows.Forms.RichTextBox();
            this.panelFlashing = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxBootloader = new System.Windows.Forms.TextBox();
            this.textBoxFirmware = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.buttonBrowseBootloader = new System.Windows.Forms.Button();
            this.buttonFlash = new System.Windows.Forms.Button();
            this.buttonBrowsePartitions = new System.Windows.Forms.Button();
            this.buttonBrowseFirmware = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.buttonBrowseBootApp = new System.Windows.Forms.Button();
            this.textBoxPartitions = new System.Windows.Forms.TextBox();
            this.textBoxBootApp = new System.Windows.Forms.TextBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.panelWaiting = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel5 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.panelConnection = new System.Windows.Forms.Panel();
            this.labelAbout = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.panelSuccess = new System.Windows.Forms.Panel();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.buttonGoBack = new System.Windows.Forms.Button();
            this.labelReport = new System.Windows.Forms.Label();
            this.panelFlashing.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.panelWaiting.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.panelConnection.SuspendLayout();
            this.panelSuccess.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // richTextBoxLog
            // 
            this.richTextBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBoxLog.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxLog.HideSelection = false;
            this.richTextBoxLog.Location = new System.Drawing.Point(8, 523);
            this.richTextBoxLog.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.richTextBoxLog.Name = "richTextBoxLog";
            this.richTextBoxLog.ReadOnly = true;
            this.richTextBoxLog.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.richTextBoxLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedBoth;
            this.richTextBoxLog.Size = new System.Drawing.Size(581, 275);
            this.richTextBoxLog.TabIndex = 2;
            this.richTextBoxLog.Text = "";
            this.richTextBoxLog.WordWrap = false;
            // 
            // panelFlashing
            // 
            this.panelFlashing.Controls.Add(this.richTextBoxLog);
            this.panelFlashing.Controls.Add(this.panel2);
            this.panelFlashing.Location = new System.Drawing.Point(472, 13);
            this.panelFlashing.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panelFlashing.Name = "panelFlashing";
            this.panelFlashing.Padding = new System.Windows.Forms.Padding(8);
            this.panelFlashing.Size = new System.Drawing.Size(597, 806);
            this.panelFlashing.TabIndex = 4;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.panel1);
            this.panel2.Controls.Add(this.pictureBox3);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(8, 8);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(581, 515);
            this.panel2.TabIndex = 15;
            // 
            // panel1
            // 
            this.panel1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.textBoxBootloader);
            this.panel1.Controls.Add(this.textBoxFirmware);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.buttonBrowseBootloader);
            this.panel1.Controls.Add(this.buttonFlash);
            this.panel1.Controls.Add(this.buttonBrowsePartitions);
            this.panel1.Controls.Add(this.buttonBrowseFirmware);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.buttonBrowseBootApp);
            this.panel1.Controls.Add(this.textBoxPartitions);
            this.panel1.Controls.Add(this.textBoxBootApp);
            this.panel1.Location = new System.Drawing.Point(-86, 321);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(754, 194);
            this.panel1.TabIndex = 15;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 125);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "Файл Firmware";
            // 
            // textBoxBootloader
            // 
            this.textBoxBootloader.Location = new System.Drawing.Point(147, 2);
            this.textBoxBootloader.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBoxBootloader.Multiline = true;
            this.textBoxBootloader.Name = "textBoxBootloader";
            this.textBoxBootloader.Size = new System.Drawing.Size(488, 34);
            this.textBoxBootloader.TabIndex = 9;
            this.textBoxBootloader.TextChanged += new System.EventHandler(this.textBoxBootloader_TextChanged);
            // 
            // textBoxFirmware
            // 
            this.textBoxFirmware.Location = new System.Drawing.Point(147, 116);
            this.textBoxFirmware.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBoxFirmware.Multiline = true;
            this.textBoxFirmware.Name = "textBoxFirmware";
            this.textBoxFirmware.Size = new System.Drawing.Size(488, 34);
            this.textBoxFirmware.TabIndex = 1;
            this.textBoxFirmware.TextChanged += new System.EventHandler(this.textBoxFirmware_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 49);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 16);
            this.label2.TabIndex = 8;
            this.label2.Text = "Файл Partitions";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(8, 87);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(98, 16);
            this.label6.TabIndex = 14;
            this.label6.Text = "Файл BootApp";
            // 
            // buttonBrowseBootloader
            // 
            this.buttonBrowseBootloader.Location = new System.Drawing.Point(641, 2);
            this.buttonBrowseBootloader.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonBrowseBootloader.Name = "buttonBrowseBootloader";
            this.buttonBrowseBootloader.Size = new System.Drawing.Size(107, 34);
            this.buttonBrowseBootloader.TabIndex = 10;
            this.buttonBrowseBootloader.Text = "Обрати";
            this.buttonBrowseBootloader.UseVisualStyleBackColor = true;
            this.buttonBrowseBootloader.Click += new System.EventHandler(this.buttonBrowseBootloader_Click);
            // 
            // buttonFlash
            // 
            this.buttonFlash.Enabled = false;
            this.buttonFlash.Image = global::esptool_gui.Properties.Resources.maintenance;
            this.buttonFlash.Location = new System.Drawing.Point(260, 154);
            this.buttonFlash.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonFlash.Name = "buttonFlash";
            this.buttonFlash.Size = new System.Drawing.Size(232, 39);
            this.buttonFlash.TabIndex = 0;
            this.buttonFlash.Text = "Прошити годинник";
            this.buttonFlash.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonFlash.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonFlash.UseVisualStyleBackColor = true;
            this.buttonFlash.Click += new System.EventHandler(this.buttonFlash_Click);
            // 
            // buttonBrowsePartitions
            // 
            this.buttonBrowsePartitions.Location = new System.Drawing.Point(641, 40);
            this.buttonBrowsePartitions.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonBrowsePartitions.Name = "buttonBrowsePartitions";
            this.buttonBrowsePartitions.Size = new System.Drawing.Size(107, 34);
            this.buttonBrowsePartitions.TabIndex = 7;
            this.buttonBrowsePartitions.Text = "Обрати";
            this.buttonBrowsePartitions.UseVisualStyleBackColor = true;
            this.buttonBrowsePartitions.Click += new System.EventHandler(this.buttonBrowsePartitions_Click);
            // 
            // buttonBrowseFirmware
            // 
            this.buttonBrowseFirmware.Location = new System.Drawing.Point(641, 116);
            this.buttonBrowseFirmware.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonBrowseFirmware.Name = "buttonBrowseFirmware";
            this.buttonBrowseFirmware.Size = new System.Drawing.Size(107, 34);
            this.buttonBrowseFirmware.TabIndex = 2;
            this.buttonBrowseFirmware.Text = "Обрати";
            this.buttonBrowseFirmware.UseVisualStyleBackColor = true;
            this.buttonBrowseFirmware.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(8, 11);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(112, 16);
            this.label5.TabIndex = 11;
            this.label5.Text = "Файл Bootloader";
            // 
            // buttonBrowseBootApp
            // 
            this.buttonBrowseBootApp.Location = new System.Drawing.Point(641, 78);
            this.buttonBrowseBootApp.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonBrowseBootApp.Name = "buttonBrowseBootApp";
            this.buttonBrowseBootApp.Size = new System.Drawing.Size(107, 34);
            this.buttonBrowseBootApp.TabIndex = 13;
            this.buttonBrowseBootApp.Text = "Обрати";
            this.buttonBrowseBootApp.UseVisualStyleBackColor = true;
            this.buttonBrowseBootApp.Click += new System.EventHandler(this.buttonBrowseBootApp_Click);
            // 
            // textBoxPartitions
            // 
            this.textBoxPartitions.Location = new System.Drawing.Point(147, 40);
            this.textBoxPartitions.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBoxPartitions.Multiline = true;
            this.textBoxPartitions.Name = "textBoxPartitions";
            this.textBoxPartitions.Size = new System.Drawing.Size(488, 34);
            this.textBoxPartitions.TabIndex = 6;
            this.textBoxPartitions.TextChanged += new System.EventHandler(this.textBoxPartitions_TextChanged);
            // 
            // textBoxBootApp
            // 
            this.textBoxBootApp.Location = new System.Drawing.Point(147, 78);
            this.textBoxBootApp.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBoxBootApp.Multiline = true;
            this.textBoxBootApp.Name = "textBoxBootApp";
            this.textBoxBootApp.Size = new System.Drawing.Size(488, 34);
            this.textBoxBootApp.TabIndex = 12;
            this.textBoxBootApp.TextChanged += new System.EventHandler(this.textBoxBootApp_TextChanged);
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackgroundImage = global::esptool_gui.Properties.Resources.boot_mode_connected_v2;
            this.pictureBox3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox3.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBox3.Location = new System.Drawing.Point(0, 0);
            this.pictureBox3.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(581, 329);
            this.pictureBox3.TabIndex = 14;
            this.pictureBox3.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(166, 0);
            this.label3.Name = "label3";
            this.label3.Padding = new System.Windows.Forms.Padding(5);
            this.label3.Size = new System.Drawing.Size(440, 35);
            this.label3.TabIndex = 9;
            this.label3.Text = "Підключіть годинник в режимі прошивки\r\n";
            // 
            // labelStatus
            // 
            this.labelStatus.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelStatus.Location = new System.Drawing.Point(8, 8);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(772, 17);
            this.labelStatus.TabIndex = 6;
            this.labelStatus.Text = "Очікую підключення годинника";
            this.labelStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panelWaiting
            // 
            this.panelWaiting.Controls.Add(this.pictureBox1);
            this.panelWaiting.Controls.Add(this.panel5);
            this.panelWaiting.Location = new System.Drawing.Point(12, 12);
            this.panelWaiting.Name = "panelWaiting";
            this.panelWaiting.Padding = new System.Windows.Forms.Padding(8);
            this.panelWaiting.Size = new System.Drawing.Size(454, 807);
            this.panelWaiting.TabIndex = 5;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::esptool_gui.Properties.Resources.boot_mode_help_v2;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(8, 8);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(438, 639);
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            // 
            // panel5
            // 
            this.panel5.Controls.Add(this.label4);
            this.panel5.Controls.Add(this.label3);
            this.panel5.Controls.Add(this.pictureBox2);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel5.Location = new System.Drawing.Point(8, 647);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(438, 152);
            this.panel5.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.ForeColor = System.Drawing.Color.DimGray;
            this.label4.Location = new System.Drawing.Point(166, 35);
            this.label4.Name = "label4";
            this.label4.Padding = new System.Windows.Forms.Padding(5);
            this.label4.Size = new System.Drawing.Size(272, 117);
            this.label4.TabIndex = 11;
            this.label4.Text = resources.GetString("label4.Text");
            // 
            // pictureBox2
            // 
            this.pictureBox2.Dock = System.Windows.Forms.DockStyle.Left;
            this.pictureBox2.Image = global::esptool_gui.Properties.Resources.loading_96p;
            this.pictureBox2.Location = new System.Drawing.Point(0, 0);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(166, 152);
            this.pictureBox2.TabIndex = 10;
            this.pictureBox2.TabStop = false;
            // 
            // panelConnection
            // 
            this.panelConnection.Controls.Add(this.labelStatus);
            this.panelConnection.Controls.Add(this.labelAbout);
            this.panelConnection.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelConnection.Location = new System.Drawing.Point(0, 834);
            this.panelConnection.Name = "panelConnection";
            this.panelConnection.Padding = new System.Windows.Forms.Padding(8);
            this.panelConnection.Size = new System.Drawing.Size(1547, 33);
            this.panelConnection.TabIndex = 6;
            // 
            // labelAbout
            // 
            this.labelAbout.Dock = System.Windows.Forms.DockStyle.Right;
            this.labelAbout.Location = new System.Drawing.Point(1215, 8);
            this.labelAbout.Name = "labelAbout";
            this.labelAbout.Size = new System.Drawing.Size(324, 17);
            this.labelAbout.TabIndex = 7;
            this.labelAbout.Text = "Розробник: Dr.Failov, 2024";
            this.labelAbout.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // panelSuccess
            // 
            this.panelSuccess.Controls.Add(this.pictureBox4);
            this.panelSuccess.Controls.Add(this.panel4);
            this.panelSuccess.Location = new System.Drawing.Point(1075, 13);
            this.panelSuccess.Name = "panelSuccess";
            this.panelSuccess.Padding = new System.Windows.Forms.Padding(8);
            this.panelSuccess.Size = new System.Drawing.Size(446, 806);
            this.panelSuccess.TabIndex = 7;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackgroundImage = global::esptool_gui.Properties.Resources.boot_mode_success;
            this.pictureBox4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox4.Location = new System.Drawing.Point(8, 8);
            this.pictureBox4.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(430, 541);
            this.pictureBox4.TabIndex = 16;
            this.pictureBox4.TabStop = false;
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label7);
            this.panel4.Controls.Add(this.label8);
            this.panel4.Controls.Add(this.buttonGoBack);
            this.panel4.Controls.Add(this.labelReport);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel4.Location = new System.Drawing.Point(8, 549);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(430, 249);
            this.panel4.TabIndex = 18;
            // 
            // label7
            // 
            this.label7.Dock = System.Windows.Forms.DockStyle.Top;
            this.label7.ForeColor = System.Drawing.Color.DimGray;
            this.label7.Location = new System.Drawing.Point(0, 126);
            this.label7.Name = "label7";
            this.label7.Padding = new System.Windows.Forms.Padding(5);
            this.label7.Size = new System.Drawing.Size(430, 75);
            this.label7.TabIndex = 19;
            this.label7.Text = "Годинник можна відключати від USB.\r\nЩоб запустити годинник, обережно натисніть го" +
    "лкою на кнопку перезавантаження.\r\nЯкщо годинник не запускається, перевірте все і" +
    " спробуйте знову.";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Dock = System.Windows.Forms.DockStyle.Top;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label8.ForeColor = System.Drawing.Color.Green;
            this.label8.Location = new System.Drawing.Point(0, 78);
            this.label8.Name = "label8";
            this.label8.Padding = new System.Windows.Forms.Padding(5);
            this.label8.Size = new System.Drawing.Size(380, 48);
            this.label8.TabIndex = 18;
            this.label8.Text = "Прошивка завершена";
            // 
            // buttonGoBack
            // 
            this.buttonGoBack.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.buttonGoBack.Image = global::esptool_gui.Properties.Resources.logout;
            this.buttonGoBack.Location = new System.Drawing.Point(103, 204);
            this.buttonGoBack.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonGoBack.Name = "buttonGoBack";
            this.buttonGoBack.Size = new System.Drawing.Size(232, 39);
            this.buttonGoBack.TabIndex = 17;
            this.buttonGoBack.Text = "Повернутись назад";
            this.buttonGoBack.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonGoBack.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonGoBack.UseVisualStyleBackColor = true;
            this.buttonGoBack.Click += new System.EventHandler(this.buttonGoBack_Click);
            // 
            // labelReport
            // 
            this.labelReport.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelReport.ForeColor = System.Drawing.Color.DarkGray;
            this.labelReport.Location = new System.Drawing.Point(0, 0);
            this.labelReport.Name = "labelReport";
            this.labelReport.Padding = new System.Windows.Forms.Padding(5);
            this.labelReport.Size = new System.Drawing.Size(430, 78);
            this.labelReport.TabIndex = 20;
            this.labelReport.Text = "Report 0001\r\nReport 0002\r\nReport 0003\r\nReport 0004";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(239)))), ((int)(((byte)(239)))), ((int)(((byte)(239)))));
            this.ClientSize = new System.Drawing.Size(1547, 867);
            this.Controls.Add(this.panelFlashing);
            this.Controls.Add(this.panelSuccess);
            this.Controls.Add(this.panelWaiting);
            this.Controls.Add(this.panelConnection);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "DRM Watch 3 Flasher v20240629";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panelFlashing.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.panelWaiting.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.panelConnection.ResumeLayout(false);
            this.panelSuccess.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonFlash;
        private System.Windows.Forms.RichTextBox richTextBoxLog;
        private System.Windows.Forms.Panel panelFlashing;
        private System.Windows.Forms.Button buttonBrowseFirmware;
        private System.Windows.Forms.TextBox textBoxFirmware;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panelWaiting;
        private System.Windows.Forms.Panel panelConnection;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label labelAbout;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button buttonBrowseBootloader;
        private System.Windows.Forms.TextBox textBoxBootloader;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonBrowsePartitions;
        private System.Windows.Forms.TextBox textBoxPartitions;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button buttonBrowseBootApp;
        private System.Windows.Forms.TextBox textBoxBootApp;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panelSuccess;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.Button buttonGoBack;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label labelReport;
    }
}

