using System;
using MyGUI.Sharp;

namespace TestApp.Sharp
{
    public class Test_ProgressBar
    {
        public static void Test()
        {
            ProgressBar progress = new ProgressBar(IntPtr.Zero, WidgetStyle.Overlapped, "Progress", new IntCoord(620, 220, 200, 16), Align.Default, "Overlapped", "");

            Align align = progress.ProgressStartPoint;
            progress.ProgressStartPoint = Align.Right;

            progress.ProgressFillTrack = !progress.ProgressFillTrack;
            progress.ProgressFillTrack = !progress.ProgressFillTrack;

            progress.ProgressRange = 100;
            progress.ProgressRange = progress.ProgressRange + 100;

            progress.ProgressPosition = 50;
            progress.ProgressPosition = progress.ProgressPosition + 1;

            progress.ProgressAutoTrack = !progress.ProgressAutoTrack;
        }
    }
}
