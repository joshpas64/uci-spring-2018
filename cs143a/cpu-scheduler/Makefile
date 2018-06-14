# Add your scheduling algorithms here!
SCHEDULING_ALGORITHMS = \
	RandomSchedulingAlgorithm.java\
	FCFSSchedulingAlgorithm.java\
	SJFSchedulingAlgorithm.java\
	PrioritySchedulingAlgorithm.java\
	RoundRobinSchedulingAlgorithm.java

# Build up SOURCES to include all java files in the package directory
PACKAGE = com/jimweller/cpuscheduler
PACKAGE_DIR = src/$(PACKAGE)
RAW_SOURCES = \
	Process.java\
	SchedulingAlgorithm.java\
	BaseSchedulingAlgorithm.java\
	OptionallyPreemptiveSchedulingAlgorithm.java\
	BetterFileFilter.java\
	CPUScheduler.java\
	MainFileLoader.java
RAW_SOURCES += $(SCHEDULING_ALGORITHMS)
SOURCES = $(foreach s, $(RAW_SOURCES), $(PACKAGE_DIR)/$(s))
# PICS_DIR = pics
BIN_DIR = bin
CLASSPATH = $(BIN_DIR):.:$(PACKAGE_DIR):lib/reflections.jar:lib/dom4j-1.6.1.jar:lib/gson-1.4.jar:lib/guava-r08.jar:lib/javassist-3.12.1.GA.jar:lib/jboss-vfs-3.0.0.CR5.jar:lib/slf4j-api-1.6.1.jar:lib/slf4j-simple-1.6.1.jar:lib/xml-apis-1.0.b2.jar

.PHONY: all jarfile submission

all: classes javadocs jarfile

classes: $(SOURCES) Makefile
# We need to tell Java compiler where to put the package hierarchy (current directory)
	mkdir -p $(BIN_DIR)
	javac -cp $(CLASSPATH) $(SOURCES) -d $(BIN_DIR)

javadocs: classes
	mkdir -p javadocs
	javadoc -private -version -author -d javadocs/ $(SOURCES) > javadocs/javadoc.log 2> javadocs/javadoc.err

jarfile: classes
	rm -f CPUSchedulingSimulator.jar
	jar -cvfm CPUSchedulingSimulator.jar src/manifest.mf -C $(BIN_DIR) $(PACKAGE) -C lib .

clean:
	rm -rf $(BIN_DIR)
	rm -rf javadocs
	rm -f *.jar
	rm -rf submission
	rm -f submission.zip

run:
	java -cp $(CLASSPATH) com.jimweller.cpuscheduler.MainFileLoader

submission:
	rm -rf submission
	rm -f submission.zip
	mkdir -p submission
	cp -r src lib submission
	cp Makefile simu.sh submission
	zip -r submission.zip submission
