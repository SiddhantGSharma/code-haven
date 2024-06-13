function onOpen() {

  var ui = SpreadsheetApp.getUi();

  ui.createMenu("Interface with User Directory")
    .addItem("Push Data to Directory", "updateUserDirectoryFromSpreadsheet")
    .addItem("Pull from Directory", "updateSpreadsheetFromUserDirectory")
    .addToUi();
}
