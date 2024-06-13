const PRODUCTION = true

const SERVICE_SCOPES = {
  DRIVE: 'https://www.googleapis.com/auth/drive',
  DIRECTORY: 'https://www.googleapis.com/auth/admin.directory.user',
  CALENDAR_EVENTS: 'https://www.googleapis.com/auth/calendar.events',
  GCP_STORAGE: 'https://www.googleapis.com/auth/devstorage.read_write',
  MAIL_SEND: 'https://www.googleapis.com/auth/gmail.send'
}

const privateKey = "**********"
const clientEmail = "***gserviceaccount.com"

function getOAuthService_(privateKey, clientEmail, subjectEmail, scopes) {
  return OAuth2.createService('drive')
    .setTokenUrl('https://accounts.google.com/o/oauth2/token')
    .setPrivateKey(privateKey)
    .setIssuer(clientEmail)
    .setSubject(subjectEmail)
    .setScope(scopes);
}
