using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace COMPort
{
    public partial class Form1 : Form
    {
        byte[] dataIn = new byte[27];
        byte gear, brake;
        UInt16 gas, bat, temp, fuel, vel, rpm;
        UInt32 od, lat, lon;

        private void lb1_Click(object sender, EventArgs e)
        {
            
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(gearSerie);
        }

        private void label15_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(brakeSerie);
        }

        private void label14_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(gasSerie);
        }

        private void label13_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(batSerie);

        }

        private void label12_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(tempSerie);
        }

        private void label7_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
           
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(odSerie);
        }

        private void label11_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(fuelSerie);
        }

        private void label10_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(velSerie);
        }

        private void label9_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
           
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(rpmSerie);
        }

        private void label6_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(lonSerie);
            lineChart1.Series.Add(latSerie);
        }

        private void label8_Click(object sender, EventArgs e)
        {
            lineChart1.Series.Remove(gearSerie);
            lineChart1.Series.Remove(brakeSerie);
            lineChart1.Series.Remove(gasSerie);
            lineChart1.Series.Remove(batSerie);
            lineChart1.Series.Remove(tempSerie);
            lineChart1.Series.Remove(fuelSerie);
            lineChart1.Series.Remove(velSerie);
            lineChart1.Series.Remove(rpmSerie);
            lineChart1.Series.Remove(odSerie);
            lineChart1.Series.Remove(latSerie);
            lineChart1.Series.Add(lonSerie);
        }

        dateTime gearSerie, brakeSerie, gasSerie, batSerie,
                tempSerie, fuelSerie, velSerie, rpmSerie, odSerie, latSerie, lonSerie;

        public Form1()
        {
            InitializeComponent();
            
            gearSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            gearSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            gearSerie.LabelInterval = 50;
            gearSerie.MinValue = 0;
            gearSerie.MaxValue = 5;
            gearSerie.Title = "gear";
            gearSerie.SupportedLabels = MindFusion.Charting.LabelKinds.XAxisLabel;

            brakeSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            brakeSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            brakeSerie.LabelInterval = 50;
            brakeSerie.MinValue = 0;
            brakeSerie.MaxValue = 5;
            brakeSerie.Title = "brake";

            gasSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            gasSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            gasSerie.LabelInterval = 50;
            gasSerie.MinValue = 0;
            gasSerie.MaxValue = 4096;
            gasSerie.Title = "gas";

            batSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            batSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            batSerie.LabelInterval = 50;
            batSerie.MinValue = 0;
            batSerie.MaxValue = 4096;
            batSerie.Title = "bat";

            tempSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            tempSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            tempSerie.LabelInterval = 50;
            tempSerie.MinValue = 0;
            tempSerie.MaxValue = 500;
            tempSerie.Title = "temp";

            fuelSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            fuelSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            fuelSerie.LabelInterval = 50;
            fuelSerie.MinValue = 0;
            fuelSerie.MaxValue = 4096;
            fuelSerie.Title = "fuel";

            velSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            velSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            velSerie.LabelInterval = 50;
            velSerie.MinValue = 0;
            velSerie.MaxValue = 500;
            velSerie.Title = "vel";

            rpmSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            rpmSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            rpmSerie.LabelInterval = 50;
            rpmSerie.MinValue = 0;
            rpmSerie.MaxValue = 500;
            rpmSerie.Title = "rpm";

            odSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            odSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            odSerie.LabelInterval = 50;
            odSerie.MinValue = 0;
            odSerie.MaxValue = 500000000;
            odSerie.Title = "od";

            latSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            latSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            latSerie.LabelInterval = 50;
            latSerie.MinValue = 0;
            latSerie.MaxValue = 5000000000;
            latSerie.Title = "lat";

            lonSerie = new dateTime(DateTime.Now, DateTime.Now, DateTime.Now.AddMinutes(1));
            lonSerie.DateTimeFormat = MindFusion.Charting.DateTimeFormat.LongTime;
            lonSerie.LabelInterval = 50;
            lonSerie.MinValue = 0;
            lonSerie.MaxValue = 500000000;
            lonSerie.Title = "lon";

            // lineChart1.Series.Add(gearSerie);
            // lineChart1.Series.Remove(gearSerie);

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            cBoxCOMPORT.Items.AddRange(ports);
        }

        private void bnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort.PortName = cBoxCOMPORT.Text;
                serialPort.BaudRate = Convert.ToInt32(cBoxBR.Text);
                serialPort.DataBits = Convert.ToInt32(cBoxDB.Text);
                serialPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cBoxSB.Text);
                serialPort.Parity = (Parity)Enum.Parse(typeof(Parity), cBoxPB.Text);

                serialPort.Open();
               progressBar.Value = 100;
            }
            catch(Exception err)
            {
                MessageBox.Show(err.Message, "Erro", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void bnClose_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Close();
                progressBar.Value = 0;
            }
        }

        private void bnSend_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
               // serialPort.Write(dataOut);

            }
        }

        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            for (int count = 0; count < 27; count++) {
                dataIn[count] = (byte)serialPort.ReadByte();
            }
            if(dataIn[0] == 0xAA)
            {
                gear = dataIn[1];                
                brake = dataIn[2];                
                gas =  (ushort) ((dataIn[3] << 8) | dataIn[4]);                
                bat =  (ushort) ((dataIn[5] << 8) | dataIn[6]);                
                temp = (ushort) ((dataIn[7] << 8) | dataIn[8]);               
                fuel = (ushort) ((dataIn[9] << 8) | dataIn[10]);                
                vel =  (ushort) ((dataIn[11] << 8) | dataIn[12]);                
                rpm =  (ushort) ((dataIn[13] << 8) | dataIn[14]);
                od =   (uint)   ((dataIn[15] << 24) | (dataIn[16] << 16) | (dataIn[17] << 8) | dataIn[18]);               
                lat =  (uint)   ((dataIn[19] << 24) | (dataIn[20] << 16) | (dataIn[21] << 8) | dataIn[22]);                
                lon =  (uint)   ((dataIn[23] << 24) | (dataIn[24] << 16) | (dataIn[25] << 8) | dataIn[26]);
                this.Invoke(new EventHandler(ShowData));
            }
            else
            {
                int dump = serialPort.ReadByte();
            }
            
        }

        private void ShowData(object sender, EventArgs e)
        {
            lbBat.Text = String.Format("{0}", bat);
            batSerie.addValue((double)bat);
            lbGas.Text = String.Format("{0}", gas);
            gasSerie.addValue((double)gas);
            lbBrake.Text = String.Format("{0}", brake);
            brakeSerie.addValue((double)brake);
            lbGear.Text = String.Format("{0}", gear);
            gearSerie.addValue((double)gear);
            lbTemp.Text = String.Format("{0}", temp);
            tempSerie.addValue((double)temp);
            lbFuel.Text = String.Format("{0}", fuel);
            fuelSerie.addValue((double)fuel);
            lbVel.Text = String.Format("{0}", vel);
            velSerie.addValue((double)vel);
            lbRPM.Text = String.Format("{0}", rpm);
            rpmSerie.addValue((double)rpm);
            lbOd.Text = String.Format("{0}", od);
            odSerie.addValue((double)od);
            lbLat.Text = String.Format("{0}", lat);
            latSerie.addValue((double)lat);
            lbLon.Text = String.Format("{0}", lon);
            lonSerie.addValue((double)lon);

            if (gearSerie.Size > 1)
            {
                double currVal = gearSerie.GetValue(gearSerie.Size - 1, 0);
                if (currVal > lineChart1.XAxis.MaxValue)
                {
                    double span = currVal - gearSerie.GetValue(gearSerie.Size - 2, 0);
                    lineChart1.XAxis.MinValue += span;
                    lineChart1.XAxis.MaxValue += span;
                }
                lineChart1.ChartPanel.InvalidateLayout();
            }
            
        }
    }
}
