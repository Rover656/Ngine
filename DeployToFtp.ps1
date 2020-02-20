Write-Host "Starting deploy"
$envConf = '{{""default"": {{""connection"": ""ftp://{0}:{1}@ftp.nerdthings.dev/docs/test""}}}}' -f $env:ftp_user, $env:ftp_password
creep -e $envConf -d '{""source"": ""hash""}' -b build/docs/docs/html -y
if ($lastExitCode -ne 0)
{
    exit -1
}
