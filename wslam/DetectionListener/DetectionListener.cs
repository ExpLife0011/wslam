using System;
using System.Threading.Tasks;
using Windows.ApplicationModel.AppService;
using Windows.ApplicationModel.Background;
using Windows.Foundation.Collections;
using Windows.UI.Notifications;
using Windows.Data.Xml.Dom;
using Windows.Foundation.Diagnostics;
using WslAntiMalwareCore;

namespace DetectionListener
{
    public sealed class DetectionListener : IBackgroundTask
    {

        AppServiceConnection _guiConnection;
        BackgroundTaskDeferral _guiDeferral;
        AppServiceConnection _serviceConnection;
        BackgroundTaskDeferral _serviceDeferral;

        int count;

        LoggingChannel channel = new LoggingChannel("my provider", null, new Guid("4bd2826e-54a1-4ba9-bf63-92b73ea1ac4a"));



        public void Run(IBackgroundTaskInstance taskInstance)
        {
            //using (var channel = new LoggingChannel("my provider", null, new Guid("4bd2826e-54a1-4ba9-bf63-92b73ea1ac4a"))) // null means use default options.
            //{

            //    channel.LogMessage("Test message", LoggingLevel.Verbose);
            //}
            _serviceDeferral = taskInstance.GetDeferral();


            taskInstance.Canceled += OnTaskCanceled;
            var details = taskInstance.TriggerDetails as AppServiceTriggerDetails;
            details.AppServiceConnection.RequestReceived += OnRequestReceived;
        }

        private void OnTaskCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            if (reason == BackgroundTaskCancellationReason.ExecutionTimeExceeded)
            {
                // log
            }
            _serviceDeferral.Complete();
        }

        async void OnRequestReceived(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            if (!args.Request.Message.ContainsKey(RequestFields.REQUEST_ID))
            {
                // log
                return;
            }

            count++;

            RequestId requestId = (RequestId)args.Request.Message[RequestFields.REQUEST_ID];
            if (requestId == RequestId.HandShake)
            {
                await HandleHandShake(sender, args);
            }
            else
            {
                await HandleRequest(requestId, sender, args);
            }
        }

        private async Task HandleHandShake(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            if (!args.Request.Message.ContainsKey(RequestFields.TOKEN))
            {
                // log
                return;
            }
            string token = args.Request.Message[RequestFields.TOKEN] as string;
            Authenticate(sender, token);

            var response = new ValueSet();
            response[RequestFields.STATUS] = count;
            await args.Request.SendResponseAsync(response);
            //var r = await sender.SendMessageAsync(response);
        }


        private void Authenticate(AppServiceConnection connection, string token)
        {
            if (!IsTokenValid(token))
            {
                return;
            }
            if (token == "WSLAMSS")
            {
                _serviceConnection = connection;
            }
            else if (token == "WslAntiMalware")
            {
                _guiConnection = connection;
            }
        }

        private bool IsTokenValid(string token)
        {
            return token != null;
        }

        private async Task HandleRequest(RequestId requestId, AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            if (!IsAuthenticated(sender))
            {
                // log
            }

            switch (requestId)
            {
                case RequestId.Detection:
                    await HandleDetection(sender, args);
                    break;
                default:
                    break;
            }
        }


        private bool IsAuthenticated(AppServiceConnection sender)
        {
            return sender == _guiConnection || sender == _serviceConnection;
        }

        async Task HandleDetection(AppServiceConnection sender, AppServiceRequestReceivedEventArgs args)
        {
            if (!args.Request.Message.ContainsKey(RequestFields.IMAGE_FILE_NAME))
            {
                // log
                return;
            }

            try
            {
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
                await _serviceConnection.SendMessageAsync(result);
            }
            catch(Exception)
            {
                var result = new ValueSet();
                result.Add("status", 0);
                await args.Request.SendResponseAsync(result);
                await _serviceConnection.SendMessageAsync(result);
            }
        }
    }
}
