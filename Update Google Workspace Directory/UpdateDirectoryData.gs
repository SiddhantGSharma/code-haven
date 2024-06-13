function updateUserDirectoryFromSpreadsheet() {
  let service = {};
  try {
    service = getOAuthService_(privateKey, clientEmail, 'richard.moreno@jerseystem.org', SERVICE_SCOPES.DIRECTORY);
    service.reset();
  } catch (error) {
    console.error('Access not granted on user', error);
    throw (error);
  }

  if (service?.hasAccess) {

    var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();

    var dataRange = sheet.getDataRange();
    var data = dataRange.getValues();

    for (var i = 1; i < data.length; i++) {
      var user = data[i][0]; // Email of the user
      var manager = data[i][1]; // Email of the user's manager
      var title = data[i][2]; // Job Title of the user
      var department = data[i][3]; // Department of the user
      var phone = data[i][4]; // Phone number of the user
      var email = data[i][5]; // Personal email of the user

      // Update the directory for the user
      try {
        updateDirectory_(service, user, manager, title, department, phone, email);
      } catch (error) {
        console.error('Error updating manager:', error);
      }
    }
  }
}

function updateDirectory_(service, user, manager, title, department, phone, email) {
  if (service?.hasAccess()) {
    // Define request body to update user directory
    const requestBody = {
      "relations": [
        {
          "value": manager,
          "type": "manager"
        }
      ],

      "phones": [
        {
          "value": phone,
          "type": "work"
        }
      ],

      "organizations": [
        {
          "title": title,
          "department": department
        }
      ],

      "emails": [
        {
          "address": email,
          "type": "home"
        }
      ]
    };

    // Define request options
    const options = {
      method: 'PUT',
      contentType: 'application/json',
      headers: {
        Authorization: 'Bearer ' + service.getAccessToken(),
      },
      payload: JSON.stringify(requestBody),
      muteHttpExceptions: true
    };

    // Construct the URL for the update request
    const baseUrl = `https://admin.googleapis.com/admin/directory/v1/users/${user}`;

    try {
      if (true) {
        // send Request
        const response = UrlFetchApp.fetch(baseUrl, options);
        
        if (response.getResponseCode() !== 200) throw new Error(`Error Updating Manager: ${response.getContentText()}`);
        
        return JSON.parse(response.getContentText());
      } else {
        console.log(`Error Updating Manager`)
      }
    } catch (error) {
      throw (error)
    }
  } else {
    console.error("No Service");
  }
}
