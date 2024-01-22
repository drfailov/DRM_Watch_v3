namespace DRM_Watch3_Melody_converter
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
            this.richTextBoxSrc = new System.Windows.Forms.RichTextBox();
            this.richTextBoxDst = new System.Windows.Forms.RichTextBox();
            this.buttonConvert = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // richTextBoxSrc
            // 
            this.richTextBoxSrc.Location = new System.Drawing.Point(44, 31);
            this.richTextBoxSrc.Name = "richTextBoxSrc";
            this.richTextBoxSrc.Size = new System.Drawing.Size(430, 483);
            this.richTextBoxSrc.TabIndex = 0;
            this.richTextBoxSrc.Text = "";
            // 
            // richTextBoxDst
            // 
            this.richTextBoxDst.Location = new System.Drawing.Point(577, 31);
            this.richTextBoxDst.Name = "richTextBoxDst";
            this.richTextBoxDst.Size = new System.Drawing.Size(544, 519);
            this.richTextBoxDst.TabIndex = 1;
            this.richTextBoxDst.Text = "";
            // 
            // buttonConvert
            // 
            this.buttonConvert.Location = new System.Drawing.Point(626, 698);
            this.buttonConvert.Name = "buttonConvert";
            this.buttonConvert.Size = new System.Drawing.Size(184, 73);
            this.buttonConvert.TabIndex = 2;
            this.buttonConvert.Text = "Convert";
            this.buttonConvert.UseVisualStyleBackColor = true;
            this.buttonConvert.Click += new System.EventHandler(this.buttonConvert_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(44, 687);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 20);
            this.label1.TabIndex = 3;
            this.label1.Text = "label1";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(994, 678);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(184, 73);
            this.button1.TabIndex = 4;
            this.button1.Text = "Optimize";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1403, 805);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonConvert);
            this.Controls.Add(this.richTextBoxDst);
            this.Controls.Add(this.richTextBoxSrc);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBoxSrc;
        private System.Windows.Forms.RichTextBox richTextBoxDst;
        private System.Windows.Forms.Button buttonConvert;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
    }
}

