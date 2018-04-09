using System;
using Windows.ApplicationModel.AppService;
using Windows.ApplicationModel.Background;
using Windows.Foundation.Collections;
using Windows.UI.Notifications;
using Windows.Data.Xml.Dom;

namespace DetectionListener
{
    public sealed class DetectionListener : IBackgroundTask
    {

        BackgroundTaskDeferral serviceDeferral;
        AppServiceConnection connection;
        static int requestCount = 0;

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            //Take a service deferral so the service isn't terminated
            serviceDeferral = taskInstance.GetDeferral();
            taskInstance.Canceled += OnTaskCanceled;


            var details = taskInstance.TriggerDetails as AppServiceTriggerDetails;
            connection = details.AppServiceConnection;

            //Listen for incoming app service requests
            connection.RequestReceived += OnRequestReceived;
        }

        private void OnTaskCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            if (serviceDeferral != null)
            {
                //Complete the service deferral
                serviceDeferral.Complete();
                serviceDeferral = null;
            }

            if (connection != null)
            {
                connection.Dispose();
                connection = null;
            }
        }

        async void OnRequestReceived(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            //Get a deferral so we can use an awaitable API to respond to the message
            requestCount += 1;
            var messageDeferral = args.GetDeferral();
            string xml =
                $@"
                <toast activationType='protocol' launch='det:'>
                    <visual>
                        <binding template='ToastGeneric'>
                            <text>Bashware detected</text>
                            <text>Path: {args.Request.Message["NT-Path"]} </text>
                            <text>UID : {args.Request.Message["UID"]} </text>
                        </binding>
                    </visual>
                </toast>";

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);
            var toast = new ToastNotification(doc);
            ToastNotificationManager.CreateToastNotifier().Show(toast);

            try
            {
                var result = new ValueSet();
                result.Add("status", 0);
                await args.Request.SendResponseAsync(result);

            }
            finally
            {
                //Complete the message deferral so the platform knows we're done responding
                messageDeferral.Complete();
            }
        }
    }
}
