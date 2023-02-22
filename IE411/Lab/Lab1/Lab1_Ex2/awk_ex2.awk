BEGIN{
    FS=" "; #field saperator in data file
}
{
    for(i=1; i<=FNR; i++){ #for all field values
        a[$3]++ #store only unique subjects
    }
}
{ 
    if($4>80){ #check if marks are greater than 80 in any subject
        b[$1]=$2 #stores student names in array 
    }
}
{
    if($3=="Chemistry"){ #check if subject is Chemistry
        c[$1]=$2; #stores student names who has chemistry subject
    }
}
{

    if($3=="History"){ #check if subject is History
        d[$3]++ #find the count of students in History
    }

}
END{
    print "-----ALL UNIQUE SUBJECTS-----"
    for(i in a){ #for each values in array
        print i #prints unique subjects
    }

    print "-----STUDENTS WITH MORE THAN 80 MARKS-----"
    for(i in b){ #for each values in array
       print b[i] #prints the students' name
    }

    print "-----STUDENTS WHO HAS CHEMISTRY SUBJECT-----"
    for(i in c){ #for each values in array
       print c[i] #prints the students' name
    }

    print "-----NUMBER OF STUDENTS WHO HAS HISTORY SUBJECT-----"
    for(i in d){ #for each values in array
        print d[i] #print count 
    }
}