using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;

namespace WslAntiMalware.Notifications
{
    public sealed class NotificationsListener : IBackgroundTask
    {
        public void Run(IBackgroundTaskInstance taskInstance)
        {
            int a = 2;
        }
    }
}
