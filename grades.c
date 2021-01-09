#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "grades.h"


struct student {
	int id;
	char *name;
	struct list *grades_of_student;

};

struct course {
	char *course_name;
	int course_grade;
};

struct grades {
	struct list *linked_list_grades;
};


int clone_func_student(void *element, void **output);
int clone_func_course(void *element, void **output);
void destroy_func_student(void *element);
void destroy_func_course(void *element);
int grades_add_student(struct grades *grades, const char *name, int id);
int grades_add_grade(struct grades *grades, 
					 const char *name,
					 int id,
					 int grade);
float grades_calc_avg(struct grades *grades, int id, char **out);
int grades_print_student(struct grades *grades, int id);
int grades_print_all(struct grades *grades);
void* clone_student(void *elem);
void* clone_course(void *elem);
void grades_destroy(struct grades *grades);


int clone_func_student(void *element, void **output) {
	
	*output = clone_student(element);
	if (!(*output)) {
		return -1;
	}
	return 0;

}


void* clone_student(void *elem) {

	struct student *old_student=(struct student*)elem;
	struct student *new_student=(struct student*)malloc(sizeof(struct student));
	if (!new_student) {
		return NULL;
	}
	new_student->id = old_student->id;
	new_student->name=(char*)malloc(sizeof(char)*(strlen(old_student->name)+1));
	if (!(new_student->name)) {
		free (new_student);
		return NULL; 
	}
	strcpy(new_student->name, old_student->name);

	new_student->grades_of_student=list_init(clone_func_course,
											 destroy_func_course);
	struct list *tmp= old_student->grades_of_student;
	struct node *tmp_node=list_begin (tmp);
	while (tmp_node) {
		list_push_back(new_student->grades_of_student, list_get(tmp_node));
		tmp_node=list_next(tmp_node);
	}
	return new_student;
}


int clone_func_course(void *element, void **output) {
	
	*output = clone_course(element);
	if (!(*output)) {
		return -1;
	}
	return 0;
}


void* clone_course(void *elem) {

	struct course *old_course = (struct course*)elem;
	struct course *new_course = (struct course*)malloc(sizeof(struct course));
	if (!new_course) {
		return NULL;
	}
	new_course->course_grade = old_course->course_grade;
	new_course->course_name=(char*)malloc(sizeof(char)*
										 (strlen(old_course->course_name)+1));
	if (!(new_course->course_name)) {
		free (new_course);
		return NULL; 
	}
	strcpy(new_course->course_name, old_course->course_name);
	return new_course;
}


void destroy_func_student(void *element) {
	struct student *arg= (struct student*)element;
	free (arg->name);
	list_destroy(arg->grades_of_student);
	free(arg);
}

void destroy_func_course(void *element) {
	struct course *arg = (struct course*)element;
	free (arg->course_name);
	free (arg);
	
}


struct grades* grades_init(){
	struct grades *all_grades=(struct grades*)malloc(sizeof(struct grades));
	if (!all_grades) {
		return NULL;
	}
	all_grades->linked_list_grades=list_init(clone_func_student,
											 destroy_func_student);
	return all_grades;
}


void grades_destroy(struct grades *grades) {
	list_destroy (grades->linked_list_grades);
	free (grades);
}

int grades_add_student(struct grades *grades, const char *name, int id){

	struct list *tmp_list;
	struct node *tmp_node;
	tmp_list= grades->linked_list_grades;
	tmp_node=list_begin(tmp_list);
	while (tmp_node) {
		if (((struct student*)(list_get(tmp_node)))->id==id) {
			return -1;
		}
		tmp_node= list_next(tmp_node);
	}


	struct student *new_student=(struct student*)malloc(sizeof(struct student));
	if (!new_student) {
		return -1;
	}

	new_student->id=id;
	new_student->name=(char*)malloc(sizeof(char)*(strlen(name) +1));
	if (!(new_student->name)) {
		free (new_student);
		return -1;
	}
	strcpy(new_student->name, name);

	new_student->grades_of_student=list_init(clone_func_course,
											 destroy_func_course);
	if (!(new_student->grades_of_student)) {
		free (new_student->name);
		free (new_student);
		return -1;
	}

	if ((list_push_back(grades->linked_list_grades, (void*)new_student))!=0) {
		free (new_student->name);
		list_destroy(new_student->grades_of_student);
		free (new_student);
		return -1;
	}
	
	free (new_student->name);
	list_destroy(new_student->grades_of_student);
	free (new_student);
	return 0;
}


int grades_add_grade(struct grades *grades,
					 const char *name,
					 int id,
					 int grade) {

	if (grades==NULL) {
		return -1;
	}

	if (grade<0 || grade>100 ||grades==NULL) {
		return -1;
	}

	/* build the node of the new course */
	struct course *new_course=(struct course*)malloc(sizeof(struct course));
	if (!new_course) {
		return -1; 
	}

	new_course->course_grade=grade;
	new_course->course_name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	if (!new_course->course_name) {
		free (new_course);
		return -1; 
	}
	strcpy(new_course->course_name, name);

	struct list *tmp_list_students=grades->linked_list_grades;
	struct node *tmp_node_student=list_begin(tmp_list_students);
	struct list *tmp_list_grades;
	struct node *tmp_node_grade;
	struct student *curr_student;
	struct course *tmp_course;
	while (tmp_node_student) {
		curr_student=(struct student*)(list_get(tmp_node_student));
		if (curr_student->id==id) {
			tmp_list_grades=curr_student->grades_of_student;

			/* check the course doesn't exists */
			tmp_node_grade=list_begin(tmp_list_grades);
			while (tmp_node_grade) {
				tmp_course = (struct course*)(list_get(tmp_node_grade));
				if (strcmp(tmp_course->course_name, name)==0) {
					free(new_course->course_name);
					free (new_course);
					return -1;
				}
				tmp_node_grade=list_next(tmp_node_grade);
			}
			list_push_back(curr_student->grades_of_student,new_course);
			free(new_course->course_name);
			free (new_course);
			return 0;		
		}
		tmp_node_student= list_next(tmp_node_student);
	}
	free (new_course->course_name);
	free (new_course);
	return -1;
}


float grades_calc_avg(struct grades *grades, int id, char **out) {

	float sum_grades=0, num_courses=0;

	if (grades==NULL) {
		out=NULL;
		return -1;
	}

	struct list *tmp_list=grades->linked_list_grades;
	struct node *tmp_node_student=list_begin(tmp_list);
	struct student *curr_student;
	struct list *tmp_grades;
	struct course *tmp_course;
	struct node *tmp_node_grade;
	while (tmp_node_student) {
		curr_student=(list_get(tmp_node_student));
		if (curr_student->id==id) {
			*out = (char*)malloc(sizeof(char)*(strlen(curr_student->name)+1));
			if (!(*out)) {
				return -1;
			}
			strcpy(*out, curr_student->name);
			tmp_grades=curr_student->grades_of_student;
			tmp_node_grade=list_begin(tmp_grades);
			while (tmp_node_grade) {
				tmp_course= (struct course*)(list_get(tmp_node_grade));
				sum_grades+=(float)(tmp_course->course_grade);
				num_courses++;
				tmp_node_grade=list_next(tmp_node_grade);
			}
				
			if (num_courses==0) {
				return 0;
			}

			return (sum_grades/num_courses);			
		}
		tmp_node_student = list_next(tmp_node_student);
	}
	out=NULL;
	return -1;
}


int grades_print_student(struct grades *grades, int id) {

	if (grades==NULL) {
		return -1;
	}

	struct list *tmp_list_students = grades->linked_list_grades;
	struct node *tmp_node_student=list_begin(tmp_list_students);
	struct list *tmp_list_grades;
	struct course *tmp_course;
	struct node *tmp_node_grade;
	struct student *curr_student;
	while (tmp_node_student) {
		curr_student=((struct student*)list_get(tmp_node_student));
		if (curr_student->id==id) {
			tmp_list_grades=curr_student->grades_of_student;
			tmp_node_grade=list_begin(tmp_list_grades);
			printf("%s %d:", curr_student->name, curr_student->id);

			while (tmp_node_grade) {
				printf(" ");
				tmp_course=(struct course*)(list_get(tmp_node_grade));
				if (list_next(tmp_node_grade)==NULL) {
					printf ("%s %d",
							tmp_course->course_name, tmp_course->course_grade);
				} else {
					printf ("%s %d,",
							tmp_course->course_name, tmp_course->course_grade);
				}
				tmp_node_grade=list_next(tmp_node_grade);
			}
			printf ("\n");
			return 0;			
		}
		tmp_node_student= list_next(tmp_node_student);
	}
	return -1;
	
}


int grades_print_all(struct grades *grades){

	if (grades==NULL)
		return -1;

	struct node *tmp_node;
	struct student *curr_student;

	tmp_node=list_begin(grades->linked_list_grades);

	while (tmp_node) {
		curr_student=(struct student*)(list_get(tmp_node));
		if (grades_print_student(grades, curr_student->id)==-1) {
			return -1;
		}
		tmp_node=list_next(tmp_node);
	}
	return 0;
}
