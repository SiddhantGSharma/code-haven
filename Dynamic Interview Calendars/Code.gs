/**
 * Dynamic Interview Calendars
 * 
 * Author: Siddhant Sharma (siddhant.sharma@jerseystem.org)
 * 
 * https://cal.jerseystem.org/interviewcalendars
 * 
 * This script fetches data from a Google Sheet and generates dynamic interview calendars for teams.
 * Add query parameter "key" with department or member name to display calendars filtered. Default "key" is "Paige Taylor"
 * E.g. https://cal.jerseystem.org/interviewcalendars?key=CAL.HUCA.LND-Paige Taylor-CAL.PODS.COD
 * 
 */
function doGet(e) {

    try {
      // Extract query parameters from the request
      const defaultParam = PropertiesService.getScriptProperties().getProperty('KEY_DF');
      const queryParameters = e.parameter.key || defaultParam; // Default key value if not provided
  
      // Split the query parameters into an array
      const keys = queryParameters.split("-");
  
      // Fetch data from the Google Sheets
      const id = PropertiesService.getScriptProperties().getProperty('ID_GS');
      const sheetName = PropertiesService.getScriptProperties().getProperty('NAME_GS');
      const sheet = SpreadsheetApp.openById(id).getSheetByName(sheetName);
      const values = sheet.getDataRange().getValues();
  
      // Initialize variables for constructing HTML content
      let encounteredUrls = new Set(); // Keep track of encountered URLs
      let iframeContainerHtml = ''; // Initialize html for container storing iframes
  
      // Initialize variables for constructing HTML content
      let iframeData = []; // Initialize array to store iframe data
  
      // Loop through each row of data
      values.forEach(row => {
        const dept = row[0]; // Get the value of the dept column (assuming it's the first column)
        const name = row[1]; // Get the value of the name column (assuming it's the second column)
        
        if (keys.includes(dept) || keys.includes(name)) { // Check if the department or name matches the query parameter values
          const url = row[2]; // Get the url from the column
          
          if (url && url.startsWith("http") && !encounteredUrls.has(url) && dept.startsWith("CAL")) { // Check if the URL is valid
            iframeData.push({ name, url }); // Push URL along with associated name key to the array
            encounteredUrls.add(url); // Add the URL to the encountered set
          }   
        }
      });
  
      // Sort iframe data alphabetically by name
      iframeData.sort((a, b) => a.name.localeCompare(b.name));
  
      iframeData.forEach(data => {
        const iframeHtml = `<iframe src="${data.url}" width="100%" height="500"></iframe>`; // Generate iframe HTML
        iframeContainerHtml += `<div class="iframe-wrapper">
                                  ${iframeHtml}
                                  <div class="name-label">${data.name}</div>
                                </div>`; // Append iframe HTML to container
      });
  
      // Generate HTML content with container for iframes
      const htmlContent = `
              <html lang="en">
                  <head>
                      <meta charset="UTF-8">
                      <meta name="viewport" content="width=device-width, initial-scale=1.0">
                      <title>Dynamic Interview Calendar</title>
  
                      <link rel="preconnect" href="https://fonts.googleapis.com">
                      <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
                      <link href="https://fonts.googleapis.com/css2?family=Quicksand:wght@553&family=Titillium+Web:ital,wght@0,200;0,300;0,400;0,600;0,700;0,900;1,200;1,300;1,400;1,600;1,700&display=swap" rel="stylesheet">
  
                      <style>
                          /* Header styles */
                          header {
                              display: flex; /* Flexbox layout */
                              margin: 10px 20px; /* Reduced margin for less horizontal spacing */
                              background-color: #384048;
                              color: #fff; /* White text color */
                              padding: 10px 30px; /* Reduced padding for a smaller header */
                              align-items: center; /* Center align items vertically */
                              justify-content: center; /* Center align content horizontally */
                              font-family: 'Titillium Web', sans-serif;
                              box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); /* Subtler shadow */
                              border-radius: 45px; /* Smaller border radius for a less pronounced curve */
                          }
  
                          /* Heading styles */
                          h1 {
                              margin: 0;
                              font-size: 24px;
                          }
  
                          /* Background Color */
                          body {
                              margin: auto 0;
                              background-color: #f2f2f2; /* Page color */
                          }

                          /* Name label styles */
                          .name-label {
                              display: flex; /* Enable flexbox */
                              justify-content: start; /* Center horizontally */
                              align-items: end; /* Center vertically */
                              height: 100%; /* Take full height of the parent */
                              text-align: center; /* Center text */
                              font-size: 16px; /* Adjust font size as needed */
                              font-weight: bold; /* Make the name bold */
                              text-transform: uppercase; /* Convert text to uppercase */
                              color: #333; /* Update text color */
                          }
                          
                          /* Add gap and slight curve to iframes */
                          .iframe-wrapper {
                              position: relative;
                              margin: 20px; /* Increase margin for more separation */
                              border-radius: 8px; /* Increase radius for a smoother curve */
                              overflow: hidden;
                              box-shadow: 0 7px 7px rgba(0, 0, 0, 0.2);
                              width: calc(33.33% - 40px); /* Set width for each iframe, considering margin */
                              background-color: #fff;
                          }

                          .iframe-wrapper .name-label {
                              position: absolute;
                              top: 0;
                              left: 0;
                              right: 0;
                              bottom: 0;
                          }
  
                          /* Remove border from iframes */
                          iframe {
                              border: none;
                          }
  
                          /* Hover effect on iframes */
                          .iframe-wrapper:hover {
                              transform: scale(1.07); /* Expand iframe slightly on hover */
                              transition: transform 0.3s ease; /* Smooth transition */
                          }
  
                          /* Flexbox container for iframes */
                          .iframe-container {
                              display: flex;
                              flex-wrap: wrap;
                              justify-content: flex-start; /* Spread items evenly */
                          }
  
                          /* Media query for smaller screens */
                          @media only screen and (max-width: 600px) {
                              header {
                                  margin: 10px 40px;
                                  padding: 15px;
                                  font-size: 18px;
                              }
  
                              h1 {
                                  font-size: 18px;
                              }
  
                              .iframe-wrapper {
                                  width: 100%; /* Full width on smaller screens */
                              }
                          }
                      </style>
                  </head>
                  <body>
                      <header>
                          <h1>Team Appointment Calendars</h1>
                      </header>
  
                      <div class="iframe-container">
                          ${iframeContainerHtml}
                      </div>
                  </body>
              </html>`;
  
      return HtmlService.createHtmlOutput(htmlContent);
    } catch (error) {
      console.error('Error fetching data:', error);
      return ContentService.createTextOutput('Internal Server Error').setStatusCode(500);
    }
}
