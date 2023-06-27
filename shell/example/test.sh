#!/bin/bash

select DRINK in tea cofee water juice appe all none tuan
do
   case $DRINK in
      tea|cofee|water|all)
         echo "Go to canteen"
         ;;
      juice|appe)
         echo "Available at home"
      ;;
      none)
         break
      ;;
	  tuan)
		 break
	   ;;
      *) echo "ERROR: Invalid selection"
      ;;
	  
   esac
done