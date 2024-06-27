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
            this.buttonFlash = new System.Windows.Forms.Button();
            this.richTextBoxLog = new System.Windows.Forms.RichTextBox();
            this.panelFlashing = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.textBoxFilePath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxCom = new System.Windows.Forms.ComboBox();
            this.panelWaiting = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.panelConnection = new System.Windows.Forms.Panel();
            this.labelAbout = new System.Windows.Forms.Label();
            this.panelLog = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.buttonLog = new System.Windows.Forms.PictureBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.panel1 = new System.Windows.Forms.Panel();
            this.panelFlashing.SuspendLayout();
            this.panelWaiting.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panelConnection.SuspendLayout();
            this.panelLog.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.buttonLog)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonFlash
            // 
            this.buttonFlash.Enabled = false;
            this.buttonFlash.Location = new System.Drawing.Point(26, 125);
            this.buttonFlash.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonFlash.Name = "buttonFlash";
            this.buttonFlash.Size = new System.Drawing.Size(462, 78);
            this.buttonFlash.TabIndex = 0;
            this.buttonFlash.Text = "Прошити плату";
            this.buttonFlash.UseVisualStyleBackColor = true;
            this.buttonFlash.Click += new System.EventHandler(this.buttonFlash_Click);
            // 
            // richTextBoxLog
            // 
            this.richTextBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBoxLog.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxLog.HideSelection = false;
            this.richTextBoxLog.Location = new System.Drawing.Point(8, 8);
            this.richTextBoxLog.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.richTextBoxLog.Name = "richTextBoxLog";
            this.richTextBoxLog.ReadOnly = true;
            this.richTextBoxLog.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.richTextBoxLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedBoth;
            this.richTextBoxLog.Size = new System.Drawing.Size(352, 852);
            this.richTextBoxLog.TabIndex = 2;
            this.richTextBoxLog.Text = "";
            this.richTextBoxLog.WordWrap = false;
            // 
            // panelFlashing
            // 
            this.panelFlashing.Controls.Add(this.label1);
            this.panelFlashing.Controls.Add(this.buttonBrowse);
            this.panelFlashing.Controls.Add(this.textBoxFilePath);
            this.panelFlashing.Controls.Add(this.buttonFlash);
            this.panelFlashing.Location = new System.Drawing.Point(1060, 111);
            this.panelFlashing.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panelFlashing.Name = "panelFlashing";
            this.panelFlashing.Padding = new System.Windows.Forms.Padding(8);
            this.panelFlashing.Size = new System.Drawing.Size(548, 264);
            this.panelFlashing.TabIndex = 4;
            this.panelFlashing.Paint += new System.Windows.Forms.PaintEventHandler(this.panel2_Paint);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(31, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(135, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "Файл для прошивки";
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(382, 40);
            this.buttonBrowse.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(107, 53);
            this.buttonBrowse.TabIndex = 2;
            this.buttonBrowse.Text = "Обрати";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // textBoxFilePath
            // 
            this.textBoxFilePath.Location = new System.Drawing.Point(26, 40);
            this.textBoxFilePath.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBoxFilePath.Multiline = true;
            this.textBoxFilePath.Name = "textBoxFilePath";
            this.textBoxFilePath.Size = new System.Drawing.Size(351, 54);
            this.textBoxFilePath.TabIndex = 1;
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
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 16);
            this.label2.TabIndex = 6;
            this.label2.Text = "COM порт";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // comboBoxCom
            // 
            this.comboBoxCom.FormattingEnabled = true;
            this.comboBoxCom.ItemHeight = 16;
            this.comboBoxCom.Location = new System.Drawing.Point(536, 15);
            this.comboBoxCom.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboBoxCom.Name = "comboBoxCom";
            this.comboBoxCom.Size = new System.Drawing.Size(618, 24);
            this.comboBoxCom.TabIndex = 3;
            // 
            // panelWaiting
            // 
            this.panelWaiting.Controls.Add(this.pictureBox1);
            this.panelWaiting.Controls.Add(this.panel5);
            this.panelWaiting.Location = new System.Drawing.Point(417, 73);
            this.panelWaiting.Name = "panelWaiting";
            this.panelWaiting.Padding = new System.Windows.Forms.Padding(8);
            this.panelWaiting.Size = new System.Drawing.Size(692, 540);
            this.panelWaiting.TabIndex = 5;
            // 
            // panel5
            // 
            this.panel5.Controls.Add(this.label4);
            this.panel5.Controls.Add(this.label3);
            this.panel5.Controls.Add(this.pictureBox2);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel5.Location = new System.Drawing.Point(8, 380);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(676, 152);
            this.panel5.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.ForeColor = System.Drawing.Color.DimGray;
            this.label4.Location = new System.Drawing.Point(166, 35);
            this.label4.Name = "label4";
            this.label4.Padding = new System.Windows.Forms.Padding(5);
            this.label4.Size = new System.Drawing.Size(510, 117);
            this.label4.TabIndex = 11;
            this.label4.Text = resources.GetString("label4.Text");
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // panelConnection
            // 
            this.panelConnection.Controls.Add(this.panel1);
            this.panelConnection.Controls.Add(this.labelAbout);
            this.panelConnection.Controls.Add(this.label2);
            this.panelConnection.Controls.Add(this.comboBoxCom);
            this.panelConnection.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelConnection.Location = new System.Drawing.Point(0, 868);
            this.panelConnection.Name = "panelConnection";
            this.panelConnection.Padding = new System.Windows.Forms.Padding(8);
            this.panelConnection.Size = new System.Drawing.Size(1537, 118);
            this.panelConnection.TabIndex = 6;
            // 
            // labelAbout
            // 
            this.labelAbout.AutoSize = true;
            this.labelAbout.Location = new System.Drawing.Point(25, 93);
            this.labelAbout.Name = "labelAbout";
            this.labelAbout.Size = new System.Drawing.Size(139, 16);
            this.labelAbout.TabIndex = 7;
            this.labelAbout.Text = "Розробник: Dr.Failov";
            // 
            // panelLog
            // 
            this.panelLog.Controls.Add(this.richTextBoxLog);
            this.panelLog.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLog.Location = new System.Drawing.Point(0, 0);
            this.panelLog.Name = "panelLog";
            this.panelLog.Padding = new System.Windows.Forms.Padding(8);
            this.panelLog.Size = new System.Drawing.Size(368, 868);
            this.panelLog.TabIndex = 7;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::esptool_gui.Properties.Resources.boot_mode_help;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(8, 8);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(676, 372);
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
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
            // buttonLog
            // 
            this.buttonLog.BackgroundImage = global::esptool_gui.Properties.Resources.list;
            this.buttonLog.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.buttonLog.Location = new System.Drawing.Point(94, 68);
            this.buttonLog.Name = "buttonLog";
            this.buttonLog.Size = new System.Drawing.Size(30, 30);
            this.buttonLog.TabIndex = 8;
            this.buttonLog.TabStop = false;
            this.toolTip1.SetToolTip(this.buttonLog, "Показати або сховати лог");
            this.buttonLog.Click += new System.EventHandler(this.buttonLog_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.buttonLog);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(1401, 8);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(128, 102);
            this.panel1.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(239)))), ((int)(((byte)(239)))), ((int)(((byte)(239)))));
            this.ClientSize = new System.Drawing.Size(1537, 986);
            this.Controls.Add(this.panelWaiting);
            this.Controls.Add(this.panelLog);
            this.Controls.Add(this.panelFlashing);
            this.Controls.Add(this.panelConnection);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "DRM Watch 3 Flasher";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panelFlashing.ResumeLayout(false);
            this.panelFlashing.PerformLayout();
            this.panelWaiting.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.panelConnection.ResumeLayout(false);
            this.panelConnection.PerformLayout();
            this.panelLog.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.buttonLog)).EndInit();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonFlash;
        private System.Windows.Forms.RichTextBox richTextBoxLog;
        private System.Windows.Forms.Panel panelFlashing;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.TextBox textBoxFilePath;
        private System.Windows.Forms.ComboBox comboBoxCom;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panelWaiting;
        private System.Windows.Forms.Panel panelConnection;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label labelAbout;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Panel panelLog;
        private System.Windows.Forms.PictureBox buttonLog;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Panel panel1;
    }
}

