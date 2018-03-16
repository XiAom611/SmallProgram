#!/bin/bash
if [[ $1 == "--help" ]]; then
  echo -e "\nreadme.txt:"
  cat readme.txt
  echo
  exit
fi
short=true
if [[ $1 == "-A" || $1 == "-a" ]]; then
  short=false
fi
echo
echo dijkstra.py:
if [[ -e dijkstra.py ]]; then
  for((i=0; i<53; i++))
  do
    python3 dijkstra.py < data/input$i.txt > dout.txt 2> err
    if ! diff -qB dout.txt data/dout$i.txt > /dev/null ; then
      if [[ -s err ]]; then
          if ! $short ;then
            echo -e "test $i: \e[1;34mRuntime Error\E(B\E[m:"
            cat err >&2
            echo >&2
          else
            echo -e "test $i: \e[1;34mRuntime Error\E(B\E[m."
          fi
      else
	        if ! $short ;then
            echo -e "test $i: \e[1;31mWrong Answer\E(B\E[m:"
            diff -B dout.txt data/dout$i.txt >&2
            echo >&2
          else
            echo -e "test $i: \e[1;31mWrong Answer\E(B\E[m."
          fi
      fi
    else
      echo -e "test $i: \e[1;32mAccepted\E(B\E[m."
    fi
    rm -f dout.txt
    rm -f err
  done
else
  echo dijkstra.py not found.
fi
echo
echo astar.py:
if [[ -e astar.py ]]; then
  for((i=0; i<53; i++))
  do
    python3 astar.py < data/input$i.txt > aout.txt 2> err
    if ! diff -qB aout.txt data/aout$i.txt > /dev/null; then
       if [[ -s err ]]; then
            if ! $short ;then
              echo -e "test $i: \e[1;34mRuntime Error\E(B\E[m:"
              cat err >&2
              echo >&2
            else
              echo -e "test $i: \e[1;34mRuntime Error\E(B\E[m."
            fi
        else
  	        if ! $short ;then
              echo -e "test $i: \e[1;31mWrong Answer\E(B\E[m:"
              diff -B aout.txt data/aout$i.txt >&2
              echo >&2
            else
              echo -e "test $i: \e[1;31mWrong Answer\E(B\E[m."
            fi
        fi
    else
      echo -e "test $i: \e[1;32mAccepted\E(B\E[m."
    fi
    rm -f aout.txt
    rm -f err
  done
else
  echo astar.py not found.
fi
echo
