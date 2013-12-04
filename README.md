OpenMP-based language detection (trigrams)
==========================================

For now this is just a homework task, hence it's not structured the way
library ought to look. However converting it into lib is considered since it
appears to be quite fast (50MB file converted to 156KB <lang>.dat file 
within 0.17s).

To collect data about languages, <stats.h> TrigramStats class is used.
It stores its stats in <lang>.dat files in current directory.
"src/analytics_test.cpp" demonstrates, how analytics works.

When language data is collected, <detector.cpp> LangDetector class is
responsible for comparism between new text feed and already stats stored in
<lang>.dat files within current directory.
"src/detection_test.cpp" demonstrates, how detection works.

Try it!
-------

Run make command. It will produce two execs:

    - analytics_test
    - detection_test

To produce language-specific <lang>.dat file, run:

    analytics_test <count> <lang> <file1> [<file2> ...]

    # count: threads count
    # lang:  language for which current run is collecting stats
    # fileN: file containing language-specific text to process by TrigramStats class

To determine language, run:

    detection_test <count> <file>

    # count: threads count
    # file: file containing text to examine

NOTE: `detection_test` should not be ran until <lang>.dat files are generated via `analytics_test`.
