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
            this.label3 = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.panelWaiting = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.panelConnection = new System.Windows.Forms.Panel();
            this.labelAbout = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonFlash = new System.Windows.Forms.Button();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.panelFlashing.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panelWaiting.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panelConnection.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // richTextBoxLog
            // 
            this.richTextBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBoxLog.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxLog.HideSelection = false;
            this.richTextBoxLog.Location = new System.Drawing.Point(8, 468);
            this.richTextBoxLog.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.richTextBoxLog.Name = "richTextBoxLog";
            this.richTextBoxLog.ReadOnly = true;
            this.richTextBoxLog.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.richTextBoxLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedBoth;
            this.richTextBoxLog.Size = new System.Drawing.Size(771, 330);
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
            this.panelFlashing.Size = new System.Drawing.Size(787, 806);
            this.panelFlashing.TabIndex = 4;
            this.panelFlashing.Paint += new System.Windows.Forms.PaintEventHandler(this.panelFlashing_Paint);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.panel1);
            this.panel2.Controls.Add(this.pictureBox3);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(8, 8);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(771, 460);
            this.panel2.TabIndex = 15;
            // 
            // panel1
            // 
            this.panel1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.panel1.Controls.Add(this.buttonSave);
            this.panel1.Controls.Add(this.buttonFlash);
            this.panel1.Location = new System.Drawing.Point(11, 359);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(754, 75);
            this.panel1.TabIndex = 15;
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
            this.label3.Size = new System.Drawing.Size(356, 35);
            this.label3.TabIndex = 9;
            this.label3.Text = "Підключіть увімкнений годинник";
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
            // panelConnection
            // 
            this.panelConnection.Controls.Add(this.labelStatus);
            this.panelConnection.Controls.Add(this.labelAbout);
            this.panelConnection.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelConnection.Location = new System.Drawing.Point(0, 834);
            this.panelConnection.Name = "panelConnection";
            this.panelConnection.Padding = new System.Windows.Forms.Padding(8);
            this.panelConnection.Size = new System.Drawing.Size(1284, 33);
            this.panelConnection.TabIndex = 6;
            // 
            // labelAbout
            // 
            this.labelAbout.Dock = System.Windows.Forms.DockStyle.Right;
            this.labelAbout.Location = new System.Drawing.Point(952, 8);
            this.labelAbout.Name = "labelAbout";
            this.labelAbout.Size = new System.Drawing.Size(324, 17);
            this.labelAbout.TabIndex = 7;
            this.labelAbout.Text = "Розробник: Dr.Failov, 2024";
            this.labelAbout.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonSave
            // 
            this.buttonSave.Enabled = false;
            this.buttonSave.Image = global::esptool_gui.Properties.Resources.save;
            this.buttonSave.Location = new System.Drawing.Point(390, 12);
            this.buttonSave.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(232, 47);
            this.buttonSave.TabIndex = 1;
            this.buttonSave.Text = "Зберегти в файл";
            this.buttonSave.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonFlash
            // 
            this.buttonFlash.Enabled = false;
            this.buttonFlash.Image = global::esptool_gui.Properties.Resources.download;
            this.buttonFlash.Location = new System.Drawing.Point(152, 12);
            this.buttonFlash.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonFlash.Name = "buttonFlash";
            this.buttonFlash.Size = new System.Drawing.Size(232, 47);
            this.buttonFlash.TabIndex = 0;
            this.buttonFlash.Text = "Отримати скріншот";
            this.buttonFlash.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonFlash.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonFlash.UseVisualStyleBackColor = true;
            this.buttonFlash.Click += new System.EventHandler(this.buttonFlash_Click);
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackgroundImage = global::esptool_gui.Properties.Resources.usb_connect_help;
            this.pictureBox3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox3.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBox3.Location = new System.Drawing.Point(0, 0);
            this.pictureBox3.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(771, 329);
            this.pictureBox3.TabIndex = 14;
            this.pictureBox3.TabStop = false;
            this.pictureBox3.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox3_Paint);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::esptool_gui.Properties.Resources.usb_connect_help;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(8, 8);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(438, 639);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(239)))), ((int)(((byte)(239)))), ((int)(((byte)(239)))));
            this.ClientSize = new System.Drawing.Size(1284, 867);
            this.Controls.Add(this.panelFlashing);
            this.Controls.Add(this.panelWaiting);
            this.Controls.Add(this.panelConnection);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "DRM Watch 3 Screenshot Tool v20240825";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form1_Paint);
            this.panelFlashing.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panelWaiting.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.panelConnection.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonFlash;
        private System.Windows.Forms.RichTextBox richTextBoxLog;
        private System.Windows.Forms.Panel panelFlashing;
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
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button buttonSave;
    }
}

