
namespace COMPort
{
    partial class Form1
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.cBoxBR = new System.Windows.Forms.ComboBox();
            this.cBoxDB = new System.Windows.Forms.ComboBox();
            this.cBoxPB = new System.Windows.Forms.ComboBox();
            this.cBoxSB = new System.Windows.Forms.ComboBox();
            this.cBoxCOMPORT = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.bnClose = new System.Windows.Forms.Button();
            this.bnOpen = new System.Windows.Forms.Button();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.lb1 = new System.Windows.Forms.Label();
            this.lbGear = new System.Windows.Forms.Label();
            this.lbBrake = new System.Windows.Forms.Label();
            this.lbGas = new System.Windows.Forms.Label();
            this.lbBat = new System.Windows.Forms.Label();
            this.lbTemp = new System.Windows.Forms.Label();
            this.lbFuel = new System.Windows.Forms.Label();
            this.lbVel = new System.Windows.Forms.Label();
            this.lbRPM = new System.Windows.Forms.Label();
            this.lbLon = new System.Windows.Forms.Label();
            this.lbOd = new System.Windows.Forms.Label();
            this.lbLat = new System.Windows.Forms.Label();
            this.lineChart1 = new MindFusion.Charting.WinForms.LineChart();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.cBoxBR);
            this.groupBox1.Controls.Add(this.cBoxDB);
            this.groupBox1.Controls.Add(this.cBoxPB);
            this.groupBox1.Controls.Add(this.cBoxSB);
            this.groupBox1.Controls.Add(this.cBoxCOMPORT);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(274, 211);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Com Port Control";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 177);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(91, 17);
            this.label5.TabIndex = 9;
            this.label5.Text = "PARITY BITS";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 142);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 17);
            this.label4.TabIndex = 8;
            this.label4.Text = "STOP BITS";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 107);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 17);
            this.label3.TabIndex = 7;
            this.label3.Text = "DATA BITS";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 72);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "BAUD RATE";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 17);
            this.label1.TabIndex = 5;
            this.label1.Text = "COM PORT";
            // 
            // cBoxBR
            // 
            this.cBoxBR.FormattingEnabled = true;
            this.cBoxBR.Items.AddRange(new object[] {
            "9600",
            "115200"});
            this.cBoxBR.Location = new System.Drawing.Point(106, 72);
            this.cBoxBR.Name = "cBoxBR";
            this.cBoxBR.Size = new System.Drawing.Size(149, 24);
            this.cBoxBR.TabIndex = 4;
            this.cBoxBR.Text = "115200";
            // 
            // cBoxDB
            // 
            this.cBoxDB.FormattingEnabled = true;
            this.cBoxDB.Items.AddRange(new object[] {
            "5",
            "6",
            "7",
            "8",
            "9"});
            this.cBoxDB.Location = new System.Drawing.Point(106, 107);
            this.cBoxDB.Name = "cBoxDB";
            this.cBoxDB.Size = new System.Drawing.Size(149, 24);
            this.cBoxDB.TabIndex = 3;
            this.cBoxDB.Text = "8";
            // 
            // cBoxPB
            // 
            this.cBoxPB.FormattingEnabled = true;
            this.cBoxPB.Items.AddRange(new object[] {
            "None",
            "Odd",
            "Even"});
            this.cBoxPB.Location = new System.Drawing.Point(106, 177);
            this.cBoxPB.Name = "cBoxPB";
            this.cBoxPB.Size = new System.Drawing.Size(149, 24);
            this.cBoxPB.TabIndex = 2;
            this.cBoxPB.Text = "None";
            // 
            // cBoxSB
            // 
            this.cBoxSB.FormattingEnabled = true;
            this.cBoxSB.Items.AddRange(new object[] {
            "One",
            "Two"});
            this.cBoxSB.Location = new System.Drawing.Point(106, 142);
            this.cBoxSB.Name = "cBoxSB";
            this.cBoxSB.Size = new System.Drawing.Size(149, 24);
            this.cBoxSB.TabIndex = 1;
            this.cBoxSB.Text = "One";
            // 
            // cBoxCOMPORT
            // 
            this.cBoxCOMPORT.FormattingEnabled = true;
            this.cBoxCOMPORT.Location = new System.Drawing.Point(106, 37);
            this.cBoxCOMPORT.Name = "cBoxCOMPORT";
            this.cBoxCOMPORT.Size = new System.Drawing.Size(149, 24);
            this.cBoxCOMPORT.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.progressBar);
            this.groupBox2.Controls.Add(this.bnClose);
            this.groupBox2.Controls.Add(this.bnOpen);
            this.groupBox2.Location = new System.Drawing.Point(12, 229);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(274, 99);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(9, 61);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(246, 23);
            this.progressBar.TabIndex = 2;
            // 
            // bnClose
            // 
            this.bnClose.Location = new System.Drawing.Point(155, 21);
            this.bnClose.Name = "bnClose";
            this.bnClose.Size = new System.Drawing.Size(75, 23);
            this.bnClose.TabIndex = 1;
            this.bnClose.Text = "CLOSE";
            this.bnClose.UseVisualStyleBackColor = true;
            this.bnClose.Click += new System.EventHandler(this.bnClose_Click);
            // 
            // bnOpen
            // 
            this.bnOpen.Location = new System.Drawing.Point(41, 21);
            this.bnOpen.Name = "bnOpen";
            this.bnOpen.Size = new System.Drawing.Size(75, 23);
            this.bnOpen.TabIndex = 0;
            this.bnOpen.Text = "OPEN";
            this.bnOpen.UseVisualStyleBackColor = true;
            this.bnOpen.Click += new System.EventHandler(this.bnOpen_Click);
            // 
            // serialPort
            // 
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(196, 456);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(32, 17);
            this.label6.TabIndex = 6;
            this.label6.Text = "Lat:";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(16, 530);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(75, 17);
            this.label7.TabIndex = 7;
            this.label7.Text = "Odometer:";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(196, 491);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(36, 17);
            this.label8.TabIndex = 8;
            this.label8.Text = "Lon:";
            this.label8.Click += new System.EventHandler(this.label8_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(196, 421);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(42, 17);
            this.label9.TabIndex = 9;
            this.label9.Text = "RPM:";
            this.label9.Click += new System.EventHandler(this.label9_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(196, 386);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(53, 17);
            this.label10.TabIndex = 10;
            this.label10.Text = "Speed:";
            this.label10.Click += new System.EventHandler(this.label10_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(196, 351);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(39, 17);
            this.label11.TabIndex = 11;
            this.label11.Text = "Fuel:";
            this.label11.Click += new System.EventHandler(this.label11_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(16, 491);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(48, 17);
            this.label12.TabIndex = 12;
            this.label12.Text = "Temp:";
            this.label12.Click += new System.EventHandler(this.label12_Click);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(16, 459);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(57, 17);
            this.label13.TabIndex = 13;
            this.label13.Text = "Battery:";
            this.label13.Click += new System.EventHandler(this.label13_Click);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(16, 421);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(38, 17);
            this.label14.TabIndex = 14;
            this.label14.Text = "Gas:";
            this.label14.Click += new System.EventHandler(this.label14_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(16, 386);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(49, 17);
            this.label15.TabIndex = 15;
            this.label15.Text = "Brake:";
            this.label15.Click += new System.EventHandler(this.label15_Click);
            // 
            // lb1
            // 
            this.lb1.AutoSize = true;
            this.lb1.Location = new System.Drawing.Point(16, 351);
            this.lb1.Name = "lb1";
            this.lb1.Size = new System.Drawing.Size(44, 17);
            this.lb1.TabIndex = 16;
            this.lb1.Text = "Gear:";
            this.lb1.Click += new System.EventHandler(this.lb1_Click);
            // 
            // lbGear
            // 
            this.lbGear.AutoSize = true;
            this.lbGear.Location = new System.Drawing.Point(105, 351);
            this.lbGear.Name = "lbGear";
            this.lbGear.Size = new System.Drawing.Size(44, 17);
            this.lbGear.TabIndex = 27;
            this.lbGear.Text = "Gear:";
            // 
            // lbBrake
            // 
            this.lbBrake.AutoSize = true;
            this.lbBrake.Location = new System.Drawing.Point(105, 386);
            this.lbBrake.Name = "lbBrake";
            this.lbBrake.Size = new System.Drawing.Size(49, 17);
            this.lbBrake.TabIndex = 26;
            this.lbBrake.Text = "Brake:";
            // 
            // lbGas
            // 
            this.lbGas.AutoSize = true;
            this.lbGas.Location = new System.Drawing.Point(105, 421);
            this.lbGas.Name = "lbGas";
            this.lbGas.Size = new System.Drawing.Size(38, 17);
            this.lbGas.TabIndex = 25;
            this.lbGas.Text = "Gas:";
            // 
            // lbBat
            // 
            this.lbBat.AutoSize = true;
            this.lbBat.Location = new System.Drawing.Point(105, 459);
            this.lbBat.Name = "lbBat";
            this.lbBat.Size = new System.Drawing.Size(57, 17);
            this.lbBat.TabIndex = 24;
            this.lbBat.Text = "Battery:";
            // 
            // lbTemp
            // 
            this.lbTemp.AutoSize = true;
            this.lbTemp.Location = new System.Drawing.Point(105, 491);
            this.lbTemp.Name = "lbTemp";
            this.lbTemp.Size = new System.Drawing.Size(48, 17);
            this.lbTemp.TabIndex = 23;
            this.lbTemp.Text = "Temp:";
            // 
            // lbFuel
            // 
            this.lbFuel.AutoSize = true;
            this.lbFuel.Location = new System.Drawing.Point(285, 351);
            this.lbFuel.Name = "lbFuel";
            this.lbFuel.Size = new System.Drawing.Size(39, 17);
            this.lbFuel.TabIndex = 22;
            this.lbFuel.Text = "Fuel:";
            // 
            // lbVel
            // 
            this.lbVel.AutoSize = true;
            this.lbVel.Location = new System.Drawing.Point(285, 386);
            this.lbVel.Name = "lbVel";
            this.lbVel.Size = new System.Drawing.Size(39, 17);
            this.lbVel.TabIndex = 21;
            this.lbVel.Text = "lbVel";
            // 
            // lbRPM
            // 
            this.lbRPM.AutoSize = true;
            this.lbRPM.Location = new System.Drawing.Point(285, 421);
            this.lbRPM.Name = "lbRPM";
            this.lbRPM.Size = new System.Drawing.Size(42, 17);
            this.lbRPM.TabIndex = 20;
            this.lbRPM.Text = "RPM:";
            // 
            // lbLon
            // 
            this.lbLon.AutoSize = true;
            this.lbLon.Location = new System.Drawing.Point(285, 491);
            this.lbLon.Name = "lbLon";
            this.lbLon.Size = new System.Drawing.Size(36, 17);
            this.lbLon.TabIndex = 19;
            this.lbLon.Text = "Lon:";
            // 
            // lbOd
            // 
            this.lbOd.AutoSize = true;
            this.lbOd.Location = new System.Drawing.Point(105, 530);
            this.lbOd.Name = "lbOd";
            this.lbOd.Size = new System.Drawing.Size(38, 17);
            this.lbOd.TabIndex = 18;
            this.lbOd.Text = "lbOd";
            // 
            // lbLat
            // 
            this.lbLat.AutoSize = true;
            this.lbLat.Location = new System.Drawing.Point(285, 456);
            this.lbLat.Name = "lbLat";
            this.lbLat.Size = new System.Drawing.Size(39, 17);
            this.lbLat.TabIndex = 17;
            this.lbLat.Text = "lbLat";
            // 
            // lineChart1
            // 
            this.lineChart1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lineChart1.LegendTitle = "Legend";
            this.lineChart1.Location = new System.Drawing.Point(0, 18);
            this.lineChart1.Name = "lineChart1";
            this.lineChart1.Padding = new System.Windows.Forms.Padding(5);
            this.lineChart1.ShowLegend = true;
            this.lineChart1.Size = new System.Drawing.Size(832, 626);
            this.lineChart1.SubtitleFontName = null;
            this.lineChart1.SubtitleFontSize = null;
            this.lineChart1.SubtitleFontStyle = null;
            this.lineChart1.TabIndex = 28;
            this.lineChart1.Text = "lineChart1";
            this.lineChart1.Theme.UniformSeriesFill = new MindFusion.Drawing.SolidBrush("#FF90EE90");
            this.lineChart1.Theme.UniformSeriesStroke = new MindFusion.Drawing.SolidBrush("#FF000000");
            this.lineChart1.Theme.UniformSeriesStrokeThickness = 2D;
            this.lineChart1.TitleFontName = null;
            this.lineChart1.TitleFontSize = null;
            this.lineChart1.TitleFontStyle = null;
            // 
            // groupBox3
            // 
            this.groupBox3.AutoSize = true;
            this.groupBox3.Controls.Add(this.lineChart1);
            this.groupBox3.Location = new System.Drawing.Point(354, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(1189, 659);
            this.groupBox3.TabIndex = 29;
            this.groupBox3.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1195, 665);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.lbGear);
            this.Controls.Add(this.lbBrake);
            this.Controls.Add(this.lbGas);
            this.Controls.Add(this.lbBat);
            this.Controls.Add(this.lbTemp);
            this.Controls.Add(this.lbFuel);
            this.Controls.Add(this.lbVel);
            this.Controls.Add(this.lbRPM);
            this.Controls.Add(this.lbLon);
            this.Controls.Add(this.lbOd);
            this.Controls.Add(this.lbLat);
            this.Controls.Add(this.lb1);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "BOMBAJA DEBUG TELEMETRY";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cBoxBR;
        private System.Windows.Forms.ComboBox cBoxDB;
        private System.Windows.Forms.ComboBox cBoxPB;
        private System.Windows.Forms.ComboBox cBoxSB;
        private System.Windows.Forms.ComboBox cBoxCOMPORT;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button bnClose;
        private System.Windows.Forms.Button bnOpen;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label lb1;
        private System.Windows.Forms.Label lbGear;
        private System.Windows.Forms.Label lbBrake;
        private System.Windows.Forms.Label lbGas;
        private System.Windows.Forms.Label lbBat;
        private System.Windows.Forms.Label lbTemp;
        private System.Windows.Forms.Label lbFuel;
        private System.Windows.Forms.Label lbVel;
        private System.Windows.Forms.Label lbRPM;
        private System.Windows.Forms.Label lbLon;
        private System.Windows.Forms.Label lbOd;
        private System.Windows.Forms.Label lbLat;
        private MindFusion.Charting.WinForms.LineChart lineChart1;
        private System.Windows.Forms.GroupBox groupBox3;
    }
}

