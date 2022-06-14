namespace COMPort
{
    public class dateTimeBase
    {

        public string GetLabel(int index, LabelKinds kind)
        {
            if (kind == LabelKinds.XAxisLabel)
            {
                if (index < values.Count && index % labelInterval == 0)
                {
                    DateTime dateTime = new DateTime(dates[index]);

                    SortedList dateTimeFormats = new SortedList(9);

                }
            }
        }
    }
}