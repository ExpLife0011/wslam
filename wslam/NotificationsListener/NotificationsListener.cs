using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.ApplicationModel.AppService;
using Windows.ApplicationModel.Background;
using Windows.Data.Xml.Dom;
using Windows.Foundation.Collections;
using Windows.Foundation.Diagnostics;
using Windows.UI.Notifications;

using WslAntiMalware;
using WslAntiMalware.Core;

namespace WslAntiMalware.Notifications
{
    public sealed class NotificationsListener : IBackgroundTask
    {
        static Dictionary<AppServiceConnection, BackgroundTaskDeferral> _connections = new Dictionary<AppServiceConnection, BackgroundTaskDeferral>();

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            var deferral = taskInstance.GetDeferral();
            var details = taskInstance.TriggerDetails as AppServiceTriggerDetails;
            Logging.Instance.LogMessage($@"Application Service new connection from {details.Name}.");
            taskInstance.Canceled += OnTaskCanceled;
            _connections.Add(details.AppServiceConnection, deferral);
            details.AppServiceConnection.RequestReceived += OnRequestReceived;
            Logging.Instance.LogMessage("Application Service new connection handled.");
        }

        private async void OnRequestReceived(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            try
            {
                if (!args.Request.Message.ContainsKey(RequestFields.REQUEST_ID))
                {
                    Logging.Instance.LogMessage($@"Invalid request, missing '{RequestFields.REQUEST_ID}' field", LoggingLevel.Error);
                    return;
                }

                int requestId = (int)args.Request.Message[RequestFields.REQUEST_ID];
                Logging.Instance.LogMessage($@"received request {requestId}");
                await HandleRequest(requestId, sender, args);
            }
            catch(Exception)
            {
            }
        }

        private void OnTaskCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            var details = sender.TriggerDetails as AppServiceTriggerDetails;
            if (_connections.ContainsKey(details.AppServiceConnection))
            {
                Logging.Instance.LogMessage($@"closing connection reason: {reason.ToString()}");
                _connections[details.AppServiceConnection].Complete();
            }
            else
            {
                Logging.Instance.LogMessage($@"closing unkown connection", LoggingLevel.Critical);
            }
        }

        private async Task HandleRequest(int requestId, AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            switch (requestId)
            {
                case NotificationId.Detection:
                    Logging.Instance.LogMessage("Handling Detection");
                    await HandleDetection(sender, args);
                    Logging.Instance.LogMessage("Handled Detection");
                    break;
                default:
                    break;
            }
        }

        private async Task HandleDetection(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            if (!args.Request.Message.ContainsKey(RequestFields.IMAGE_FILE_NAME))
            {
                Logging.Instance.LogMessage($@"Invalid request, missing '{RequestFields.IMAGE_FILE_NAME}' field", LoggingLevel.Error);

                return;
            }

            string xml =
                $@"
            <toast>
                <visual>
                    <binding template='ToastGeneric'>
                        <text>Bashware detected</text>
                        <text>Executable Name: {args.Request.Message[RequestFields.IMAGE_FILE_NAME]}</text>
                    </binding>
                </visual>
            </toast>";

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);
            var toast = new ToastNotification(doc);
            ToastNotificationManager.CreateToastNotifier().Show(toast);
            var result = new ValueSet();
            result.Add(RequestFields.STATUS, 0);
            await args.Request.SendResponseAsync(result);
        }
    }
}
