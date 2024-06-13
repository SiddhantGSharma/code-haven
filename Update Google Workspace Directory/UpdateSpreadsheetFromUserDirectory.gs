function updateSpreadsheetFromUserDirectory() {
  let service = {};
  try {
    service = getOAuthService_(privateKey, clientEmail, 'richard.moreno@jerseystem.org', SERVICE_SCOPES.DIRECTORY);
    service.reset();
  } catch (error) {
    console.error('Access not granted on user', error);
    throw (error);
  }

  if (service?.hasAccess) {

    var spreadsheetId = PropertiesService.getScriptProperties().getProperty('SPREADSHEET_ID');
    var sheetName = PropertiesService.getScriptProperties().getProperty('SHEET_NAME');
    
    var sheet = SpreadsheetApp.openById(spreadsheetId).getSheetByName(sheetName);

    var dataRange = sheet.getDataRange();
    var data = dataRange.getValues();

    for (var i = 1; i < data.length; i++) {
      var user = data[i][0];// JerseySTEM email of the user

      // Retrieve data for the user from the directory
      try {
        var userData = getUserData_(service, user);
        if (userData) {
          // Update the spreadsheet with directory data
          sheet.getRange(i+1, 2).setValue(userData.manager);
          sheet.getRange(i+1, 3).setValue(userData.title);
          sheet.getRange(i+1, 4).setValue(userData.department);
          sheet.getRange(i+1, 5).setValue(userData.phone);
          sheet.getRange(i+1, 6).setValue(userData.email);
        } else {
          console.error('User data not found for', user);
        }
      } catch (error) {
        console.error('Error retrieving user data:', error);
      }
    }
  }
}

function getUserData_(service, user) {
  if (service?.hasAccess()) {
    // Construct the URL for fetching user data
    const baseUrl = `https://admin.googleapis.com/admin/directory/v1/users/${user}`;

    // Define request options
    const options = {
      method: 'GET',
      contentType: 'application/json',
      headers: {
        Authorization: 'Bearer ' + service.getAccessToken(),
      },
      muteHttpExceptions: true
    };

    try {
      const response = UrlFetchApp.fetch(baseUrl, options);

      if (response.getResponseCode() === 200) {
        const userData = JSON.parse(response.getContentText());

        return {
          manager: userData?.relations?.[0]?.value,
          title: userData?.organizations?.[0]?.title,
          department: userData?.organizations?.[0]?.department,
          phone: userData?.phones?.[0]?.value,
          email: userData?.emails?.[0]?.address
        };

      } else {
        console.error(`Error fetching user data for ${user}:`, response.getContentText());
        return null;
      }
    } catch (error) {
      console.error('Error fetching user data:', error);
      return null;
    }
  } else {
    console.error("No Service");
    return null;
  }
}
