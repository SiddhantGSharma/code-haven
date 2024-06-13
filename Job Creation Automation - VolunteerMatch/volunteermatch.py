from decouple import config
import time
from selenium.common.exceptions import WebDriverException
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
import requests

from scraper import Scraper

def get_job_listings():
    try:
        # The data payload that will be sent to the Google Sheet
        parameters = {
            "sheetUrl": config('JOB_LIST_SHEET_URL'),
            "worksheetName":config('JOB_LIST_WORKSHEET_NAME'),
        }
        
        response = requests.get(config('READ_FROM_SHEETS_API'), params=parameters)
        response = response.json()
        
        job_listings = []
        for item in response["data"]:
            # The data payload for each job listing
            job_ = {
                "title": item['Give your Opportunity a title: *'] if 'Give your Opportunity a title: *' in item else "",
                "contact": item['Select an Opportunity contact:'] if 'Select an Opportunity contact:' in item else "",
                "location": item['OPPORTUNITY LOCATION'] if 'OPPORTUNITY LOCATION' in item else "",
                "date_time": item['DATE & TIME'] if 'DATE & TIME' in item else "",
                "volunteers_needed": item['How many volunteers do you need for the Opportunity? * Estimated number: '] if 'How many volunteers do you need for the Opportunity? * Estimated number: ' in item else "",
                "has_shifts": item['SCHEDULE\nDoes this Opportunity have shifts?'] if 'SCHEDULE\nDoes this Opportunity have shifts?' in item else "",
                "covid_specific": item['OPPORTUNITY DESCRIPTION\nIs this opportunity directly addressing the impacts of COVID-19? If so, please be specific\nin your description.'] if 'OPPORTUNITY DESCRIPTION\nIs this opportunity directly addressing the impacts of COVID-19? If so, please be specific\nin your description.' in item else "",
                "description": item['Write a brief description of your Opportunity: *'] if 'Write a brief description of your Opportunity: *' in item else "",
                "requirements": item["What are the requirements for the Opportunity?\n Driver's License Needed Background Check Orientation or TrainingMinimum Age: \n"] if "What are the requirements for the Opportunity?\n Driver's License Needed Background Check Orientation or TrainingMinimum Age: \n" in item else "",
                "media": item['MEDIA'] if 'MEDIA' in item else "",
                "youtube_link": item['Add a YouTube video link'] if 'Add a YouTube video link' in item else "",
                "related_causes": item['HELP VOLUNTEERS FIND YOUR LISTING\nWhat causes does this Opportunity relate to?'] if 'HELP VOLUNTEERS FIND YOUR LISTING\nWhat causes does this Opportunity relate to?' in item else "",
                "keywords": item['Keywords:\nInclude key terms relating to your Opportunity that volunteers might search for (Comma separated).'] if 'Keywords:\nInclude key terms relating to your Opportunity that volunteers might search for (Comma separated).' in item else "",
                "special_groups": item['Does this Opportunity accommodate special groups?\n Kids\n\n Teens\n\n 55+\n\n Volunteer Groups'] if 'Does this Opportunity accommodate special groups?\n Kids\n\n Teens\n\n 55+\n\n Volunteer Groups' in item else ""
            }
            
            job_listings.append(job_)

        return job_listings            
    
    except Exception as e:
        print(f"An error occurred during getting job listings: {str(e)}")
        error = Exception("Exception during getting job listings")
        raise error

class VolunteerMatchLogin(Scraper):

    def __init__(self, email, password):
        super().__init__()
        self.email = email
        self.password = password
    
    def login(self):
        try:
            self.driver.get(self.BASE_URL)
            self.wait_for_clickable(By.ID, "global_login_link").click()
            self.wait_for_element(By.ID, "login_username").send_keys(self.email)
            self.wait_for_element(By.ID, "login_password").send_keys(self.password)
            self.wait_for_clickable(By.ID, "newSubmitButton").click()
            print("Logged in successfully")

        except Exception as e:
            print(f"An error occurred during login: {str(e)}")
            error = Exception("Exception during login")
            raise error
    
    def get_existing_job_titles(self):
        # Extract data from the current page
        def extract_job_titles():
            job_titles = []
            jobs = self.driver.find_elements(By.XPATH, '//*[@id="opp.id"]')

            for job in jobs:
                try:
                    # Extract job title from each row
                    opportunity = job.find_element(By.CLASS_NAME, 'acct--opp-dash__title').text 
                except Exception as e:
                    print(f"An error occurred: {e}. Skipping this row...")
                    continue
                
                job_titles.append(opportunity)

            return job_titles

        existing_job_titles = []
        time.sleep(5)

        try:
            existing_job_titles.extend(extract_job_titles())
            # Check if there is a "Next" button and if it is enabled, click
            next_button = self.wait_for_clickable(By.XPATH, '//*[@id="listingtable"]/div/a')
            # For loop handles the case of different XPATHs for "Next" clickable on different pages
            if next_button:
                self.driver.execute_script("arguments[0].click();", next_button)
                time.sleep(5)
                # Loop through all pages
                while True:
                    existing_job_titles.extend(extract_job_titles())
                    try:
                        next_button = self.wait_for_clickable(By.XPATH, '//*[@id="listingtable"]/div/a[2]')
                        if(next_button):
                            self.driver.execute_script("arguments[0].click();", next_button)
                            time.sleep(5)
                        else:
                            break

                    except:
                        break  # Exit the loop if there is no "Next" button

        except:
            return existing_job_titles
        
        return existing_job_titles
        
    def create_job_on_vonlunteermatch(self, job_listing):
        try:
            # Click on "Add an opportunity"
            self.wait_for_clickable(By.XPATH, '//*[@id="actionbar"]/div[2]/a[1]').click()
            # Add opportunity title
            self.wait_for_element(By.XPATH, '//*[@id="titleData.title"]').send_keys(job_listing["title"]) 
            # Will usually be virtual only opportunity
            self.wait_for_clickable(By.XPATH, '//*[@id="virtual_only"]').click() 

            # Will usually be an ongoing opportunity 
            toggle = self.driver.find_element(By.XPATH, "//span[@class='select_box_check_container timeData_select_box_container deactive']")
            self.driver.execute_script("arguments[0].click();", toggle)

            # Add estimated number of volunteers required
            self.wait_for_element(By.XPATH, '//*[@id="estVolunteers"]').send_keys("99")
            # Covid-19 response is usually "no"
            self.wait_for_clickable(By.XPATH, '//*[@id="not_c19_response"]').click()

            # Add opportunity description in iframe
            self.switch_to_frame("descriptionData.description_ifr")
            self.wait_for_element(By.XPATH, '//*[@id="tinymce"]').send_keys(job_listing["description"]) 
            self.driver.switch_to.parent_frame()

            # Only add minimum age in requirements
            self.wait_for_element(By.XPATH, '//*[@id="min_age"]').send_keys(job_listing["requirements"]) 

            # Logo and youtube link remain same for all opportunities
            self.wait_for_clickable(By.XPATH, '//*[@id="mediaData.existing_images"]').click()
            self.wait_for_clickable(By.XPATH, '//*[@id="mediaData.existing_images"]/option[2]').click()
            self.wait_for_clickable(By.XPATH, '//*[@id="mediaData.existing_videos"]').click()
            self.wait_for_clickable(By.XPATH, '//*[@id="mediaData.existing_videos"]/option[2]').click()

            # Cause categories remain same for all opportunities
            self.wait_for_clickable(By.XPATH, '//*[@id="cause_category_child_3"]').click()
            self.wait_for_clickable(By.XPATH, '//*[@id="cause_category_child_33"]').click()
            self.wait_for_clickable(By.XPATH, '//*[@id="cause_category_child_15"]').click()

            # Add opportunity keywords
            self.wait_for_element(By.XPATH, '//*[@id="keywords"]').send_keys(job_listing["keywords"])
            # Usually only click on accomodates 55+ groups
            self.wait_for_clickable(By.XPATH, '//*[@id="senior_friendly"]').click()

            # Save opportunity as draft
            self.wait_for_clickable(By.XPATH, '//*[@id="save_where_button"]').click()
            time.sleep(5)

            # Go back to dashboard
            self.wait_for_clickable(By.XPATH, '//*[@id="location"]/div/a').click()
            time.sleep(5)
            
            print("New job saved in draft successfully")
            
        except Exception as e:
            print(f"An error occurred during creation of job: {str(e)}")
            error = Exception("Exception during creation of job")
            raise error
          
email = config("USERNAME")
password = config("PASSWORD")

try:
    job_listings = get_job_listings()
    automation = VolunteerMatchLogin(email, password)
    automation.login()
    existing_job_titles = automation.get_existing_job_titles()

    for job_listing in job_listings:
        if job_listing['title'] not in existing_job_titles:
            automation.create_job_on_vonlunteermatch(job_listing)
    
except Exception as e:
    print(f"An error occurred during the automation process: {str(e)}")
    error = Exception("Exception during automation process")
    raise error

finally:
    automation.close_driver()
      
