#!/bin/bash

scp ~/builds/VHSzHf_S/0/students/DO6_CICD.ID_356283/chaurodr_student.21_school.ru/DO6_CICD-0/src/cat/s21_cat chaurodr@10.10.0.2:/usr/local/bin/
scp /var/lib/gitlab-runner/builds/VHSzHf_S/0/students/DO6_CICD.ID_356283/chaurodr_student.21_school.ru/DO6_CICD-0/src/grep/s21_grep chaurodr@10.10.0.2:/usr/local/bin/
ssh chaurodr@10.10.0.2 ls /usr/local/bin