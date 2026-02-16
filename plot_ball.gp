set datafile separator ","
set title "XR Simulation: User Perspective"
set xlabel "X"
set ylabel "Z"
set zlabel "Y (Height)"

# Draw a small arrow representing where the User is looking
set arrow from 0,0,1.8 to 0,2,1.8 head filled lc rgb "blue"

# Plot Ball Path and User Position
splot "simulation_data.csv" using 3:5:4 with lines lw 2 title "Ball Trajectory", \
      "< echo '0 0 1.8'" with points pt 7 ps 2 lc rgb "red" title "HMD Position"

pause -1 "Plesae press any Key to exit"
