lcov --capture --directory build/src --exclude '/usr/*' --exclude '9/*' --output-file coverage.info
genhtml coverage.info --output-directory out
# xdg-open out/index.html 
