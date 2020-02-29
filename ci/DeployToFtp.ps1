if ($env:APPVEYOR_REPO_NAME -ne "NerdThings/Ngine") {
exit 0
}

if ($env:APPVEYOR_REPO_BRANCH -eq "master" -or $env:APPVEYOR_REPO_BRANCH -eq "develop") {
    Write-Host "Starting deploy..."
    $envConf = '{{""default"": {{""connection"": ""ftp://{0}:{1}@ftp.nerdthings.dev/{2}""}}}}' -f $env:ftp_user, $env:ftp_password, $env:APPVEYOR_REPO_BRANCH
    creep -e $envConf -d '{""source"": ""hash""}' -b build/docs/docs/html -y
    if ($lastExitCode -ne 0)
    {
        exit -1
     }
}
