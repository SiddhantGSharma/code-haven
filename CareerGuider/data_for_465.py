from dataclasses import dataclass
from typing import List, Any
from utils.utils import *
import numpy as np
import random


@dataclass
class User:
    id: Any
    name: Any
    favorite_course_ids: Any
    favorite_user_ids: Any
    ongoing_course_ids: Any
    contact_methods: Any
    career_ids: List
    year: Any = ""
    major: Any = ""
    about_me: Any = ""

    def get_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "year": self.year,
            "major": self.major,
            "about_me": self.about_me,
            "favorite_course_ids": self.favorite_course_ids,
            "favorite_user_ids": self.favorite_user_ids,
            "ongoing_course_ids": self.ongoing_course_ids,
            "contact_methods": self.contact_methods,
            "career_ids": self.career_ids,
        }


@dataclass
class Course:
    id: Any
    name: Any
    interview_score_dict: Any
    growth_score_dict: Any
    skills: Any
    review_ids: Any

    def get_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "interview_score_dict": self.interview_score_dict,
            "growth_score_dict": self.growth_score_dict,
            "skills": self.skills,
            "review_ids": self.review_ids,
        }


@dataclass
class Career:
    id: Any
    name: Any
    relevant_courses: Any = None

    def get_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "relevant_courses": self.relevant_courses,
        }


@dataclass
class Review:
    id: Any  # start at 300
    user_id: Any = None
    course_id: Any = None
    text: Any = ""
    interview_score: Any = None
    growth_score: Any = None

    def get_dict(self):
        return {
            "id": self.id,
            "user_id": self.user_id,
            "course_id": self.course_id,
            "text": self.text,
            "interview_score": self.interview_score,
            "growth_score": self.growth_score,
        }


def create_random_data():
    careers = [Career(id=0, name="Machine Learning Engineer", relevant_courses=[0, 1, 2]),
               Career(id=1, name="Data Scientist", relevant_courses=[0, 1, 2]),
               Career(id=2, name="Software Engineer", relevant_courses=[3,4]),
               Career(id=3, name="Data Engineer", relevant_courses=[0, 1, 2]),
               Career(id=4, name="Data Analyst", relevant_courses=[0, 1, 2]), ]

    course_names = ["Intro to Machine Learning", "Advanced NLP", "Intro to Deep Learning",
                    "Intro to Data Structure", "Introduction to Algorithm"]  # v, "Discrete Math"
    courses = [Course(id=i, name=course_name,
                      interview_score_dict={i: round(random.uniform(3, 5), 1) for i in range(5)},
                      growth_score_dict={i: round(random.uniform(3, 5), 1)  for i in range(5)},
                      skills=["Matrix Math", "AI Algorithm", "Coding"], review_ids=[0, 1]) for i, course_name in enumerate(course_names)] + \
              [Course(id=i + 3, name=course_name,
                      interview_score_dict={i: round(random.uniform(3, 5), 1)  for i in range(5)},
                      growth_score_dict={i: round(random.uniform(3, 5), 1)  for i in range(5)},
                      skills=["Coding", "Data Structures", "Discrete Math", ], review_ids=[2, 3]) for i, course_name in enumerate(course_names)]

    users = [User(id=0, name="Alice", favorite_course_ids=[0, 1], favorite_user_ids=[1, 2],
                  ongoing_course_ids=[0, 1], career_ids=[0, 1], about_me="I am a data scientist",
                  contact_methods=["email", "phone"], year="junior", major="Computer Science"),
             User(id=1, name="Bob", favorite_course_ids=[0, 1], favorite_user_ids=[0, 2],
                  ongoing_course_ids=[0, 1], career_ids=[0, 1], about_me="I am a machine learning engineer",
                  contact_methods=["email"], year="freshman", major="Computer Science"),
             User(id=2, name="Charlie", favorite_course_ids=[0, 1], favorite_user_ids=[0, 1],
                  ongoing_course_ids=[0, 1], career_ids=[0, 1], about_me="Electric Engineer",
                  contact_methods=[], year="senior", major="Computer Science")]

    reviews = [Review(id=0, user_id=0, course_id=0, text="Great course!", interview_score=4.5, growth_score=4.5),
               Review(id=1, user_id=1, course_id=1, text="Great course!", interview_score=4.5, growth_score=4.5),
               Review(id=2, user_id=0, course_id=2, text="Great course!", interview_score=4.5, growth_score=4.5),
               Review(id=3, user_id=2, course_id=3, text="Great course!", interview_score=4.5, growth_score=4.5), ]

    final_data = {
        "users": [user.get_dict() for user in users],
        "courses": [course.get_dict() for course in courses],
        "careers": [career.get_dict() for career in careers],
        "reviews": [review.get_dict() for review in reviews],
    }
    dump_file(final_data, "data.json")
    # return final_data
create_random_data()