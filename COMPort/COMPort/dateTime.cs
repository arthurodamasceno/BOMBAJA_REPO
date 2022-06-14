using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MindFusion.Charting;

namespace COMPort
{
    public class dateTime : Series
    {
        public dateTime(DateTime start, DateTime minDate, DateTime maxDate)
        {
            this.start = start;
            this.minDate = minDate;
            this.maxDate = maxDate;

            dateTimeFormat = DateTimeFormat.ShortTime;
            customDateTimeFormat = "";
            labelInterval = 10;

            dates = new List<long>();
            values = new List<double>();
        }
        public int Size
        {
            get { return values.Count; }
        }


        public int Dimensions
        {
            get { return 2; }
        }

        public string Title
        {
            get; set;
        }

        public LabelKinds SupportedLabels
        {
            get { return labelKinds; }
            set { labelKinds = value; }
        }
        public event EventHandler DataChanged;

        protected virtual void OndDataChanged()
        {
            if (DataChanged != null)
                DataChanged(this, EventArgs.Empty);
        }

        public string GetLabel(int index, LabelKinds kind)
        {
            if(kind == LabelKinds.XAxisLabel)
            {
                if (index < values.Count && index % labelInterval == 0)
                {
                    DateTime dateTime = new DateTime(dates[index]);

                    SortedList dateTimeFormats = new SortedList(9);
                    dateTimeFormats.Add("d", DateTimeFormat.ShortDate);
                    dateTimeFormats.Add("D", DateTimeFormat.LongDate);
                    dateTimeFormats.Add("t", DateTimeFormat.ShortTime);
                    dateTimeFormats.Add("T", DateTimeFormat.LongTime);
                    dateTimeFormats.Add("M", DateTimeFormat.MonthDateTime);
                    dateTimeFormats.Add("Y", DateTimeFormat.YearDateTime);
                    dateTimeFormats.Add("f", DateTimeFormat.FullDateTime);
                    dateTimeFormats.Add("*", DateTimeFormat.CustomDateTime);
                    dateTimeFormats.Add("", DateTimeFormat.None);

                    string format = customDateTimeFormat;

                    if(dateTimeFormat != DateTimeFormat.None &&
                        dateTimeFormat != DateTimeFormat.CustomDateTime)
                    {
                        int fIndex = dateTimeFormats.IndexOfValue(dateTimeFormat);
                        format = dateTimeFormats.GetKey(fIndex).ToString();
                    }

                    return dateTime.ToString(format);
                }
            }
            return "";

        }

        public double GetValue(int index, int dimension)
        {
            if(dimension == 0)
            {
                if(index < dates.Count && index >= 0)
                {
                    long currValue = dates[index];
                    var p = (currValue - (double)minDate.Ticks) / ((double)maxDate.Ticks - (double)minDate.Ticks);

                    return minValue + ((maxValue - minValue) * p);
                }
            }

            if(dimension == 1)
            {
                return values[index];
            }
            return -1;
        }

        public bool IsEmphasized(int index)
        {
            return false;
        }

        public bool IsSorted(int dimension)
        {
            return dimension == 0;
        }

        public void addValue(double value)
        {
            this.values.Add(value);
            long currTime = DateTime.Now.Ticks;
            dates.Add(currTime);
        }

        public DateTimeFormat DateTimeFormat
        {
            get { return dateTimeFormat; }
            set
            {
                if (dateTimeFormat == value) return;
                dateTimeFormat = value;
                OndDataChanged();
            }
        }

        public int LabelInterval
        {
            get { return labelInterval; }
            set{
                if (labelInterval == value) return;
                labelInterval = value;
                OndDataChanged();
            }
        }

        public string CustomDateTimeFormat
        {
            get { return customDateTimeFormat; }
            set
            {
                if (customDateTimeFormat == value) return;
                customDateTimeFormat = value;
                OndDataChanged();
            }
        }

        public DateTime MinDate
        {
            get { return minDate; }
            set
            {
                if (minDate == value) return;
                minDate = value;
                OndDataChanged();
            }
        }

        public DateTime MaxDate
        {
            get { return maxDate; }
            set
            {
                if (maxDate == value) return;
                maxDate = value;
                OndDataChanged();
            }
        }

        public double MinValue
        {
            get { return minValue; }
            set
            {
                if (minValue == value) return;
                minValue = value;
                OndDataChanged();
            }
        }

        public double MaxValue
        {
            get { return maxValue; }
            set
            {
                if (maxValue == value) return;
                maxValue = value;
                OndDataChanged();
            }
        }

        DateTime start;
        List<double> values;
        List<long> dates;
        private DateTime minDate;
        private DateTime maxDate;

        private double minValue = 0;
        private double maxValue = 1;

        private int labelInterval;
        private DateTimeFormat dateTimeFormat;
        private string customDateTimeFormat;
        private LabelKinds labelKinds;
    }
}
