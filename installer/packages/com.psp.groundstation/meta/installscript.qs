function Component()
{
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
        if (installer.value("os") == "win") { 
            try {
                component.addOperation("CreateShortcut", "@TargetDir@/GroundStation.exe", "@StartMenuDir@/GroundStation.lnk", "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/appico.ico");
            } catch (e) {
                // Do nothing if key doesn't exist
            }
        }
    } catch (e) {
        print(e);
    }
}
