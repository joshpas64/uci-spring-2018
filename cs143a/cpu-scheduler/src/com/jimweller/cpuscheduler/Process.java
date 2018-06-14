package com.jimweller.cpuscheduler;

import java.io.*;

/** 
 *   An aggregate data type to represent a process to schedule. It will
 *   maintain all of it's state internally.
 *   @author Jim Weller
 */
public class Process{
    
    /** The process' identification number. It must be unique. */
    long PID=0;      // [ 0 - sizeof(long) ]

    /** Store the value of the next pid to use. There is no garbage
	collection. I just take the next available number. It is the same
	variable in *all* instances of a process.
    */
    static long nextPID = 1000; // [ 0 - sizeof(long) ]


    /** Process' CPU burst time. The total amount of CPU time that a process
	needs in seconds. [0 - 100]. */
    long burst=0;    // [0 - 100]

    /** Store original burst state. To show total an remaining I want to   
	know what the original CPU burst of a process was. The variable
	'burst' will store the "active" state 
    */
    long initBurst=0;

    /** Delay of arrival. How long after the previous process arrival
	does this process arrive? It is a time offset in seconds. 
	[0 - 95]*/
    long delay=0;    // [0 - 95]

    /** Execution priority. The weight that this process has. It aids
	the scheduler in some premptive algorithms. [0(high) - 9 (low)] */
    long priority=0; // [0(high) - 9 (low)]


    /** The actual time the process arrived. This will be set by the 
	scheduler. */
    long arrival=0;  // set by scheduler

    /** The time that the process firsts begins execution. This will be
	set by the scheduler. */
    long start=0;   

    /** The time that the process ends execution. This will be
	set by the scheduler. */
    long finish=0;  

    /** The total amount of time the process spent waiting. Measured
	from the time it arrives to the time it finishes */
    long wait=0;   
    
    /** The measure of time after arrival that it took to begin execution */
    long response=0; 

    /** Measure of the total time a process was in the clutches of the
	scheduler. Whether ready or waiting or running the full lifecycle
	of this process */
    long lifetime = 0;
    
    /** a way to check if a process has occured yet */
    boolean arrived = false;

    /** A way to check if a process has started running yet. */
    boolean started=false;

    /** A way to see if a process is complete */
    boolean finished=false;

    /** A way to see if a process is scheduled to run this cycle */
    boolean active = false;
    
  
    /** Default constructor. Randomly generate a process and fills in fields
	using the bounds specified above. */
    Process(){
	nextPID++;
	PID = nextPID;
	burst = (long) (Math.random() * 99 + 1 );
	initBurst = burst;
	delay = (long) (Math.random() * 70 );
	priority = (long) Math.round((Math.random() * 9));
    }


    /** Articulate constructor. No random generation is done on the
	programmers behalf. It is assumed that all values will be
	within the above parameters.  This method is useful for
	building a queue of process where the data comes from a file
	or other source
	@param b The burst time of the process.
	@param d The delay in process arrival mesured from the 
	         arrival of the previous process.
	@param p The priority weight of the process.
    */
    Process(long b, long d, long p){
	nextPID++;
	PID = nextPID;
	burst = b;
	initBurst = burst;
	delay = d;
	priority = p;
    }


    /**
     * Go through the motions of running one cycle on a process. 
     * uses current time to check if certain events have occured
     * (e.g. arrival, start, finish) and then sets the state of those
     * events booleans.
     */
    public  synchronized void executing(long timeNow){
	
	active=true;

	if( timeNow == arrival ){
	    arrived = true;
	}
	    
	if( burst == initBurst){
	    started  = true;
	    start    = timeNow;
	    response = start - arrival;
	}
	    
	burst--;
	lifetime++;
	    
	if( burst == 0){
	    finished = true;
	    finish = timeNow;
	}
    }


    /** 
     * The inverse of executing.  Go through the motions of waiting
     * for cpu time. Use current time to check if this was our arrival time.
     */
    public synchronized void waiting(long timeNow){
 	active=false;
	lifetime++;
	wait++;
	if( timeNow == arrival ){
	    arrived = true;
	}
    }

    
    /** Show state of process on the terminal
     */
    public void print(){
	System.out.println("PID     : " + PID + "\n" +
			   "Burst   : " + burst + "\n" +
			   "IBurst  : " + initBurst + "\n" +
			   "Delay   : " + delay + "\n" +
			   "Priority: " + priority + "\n" +
			   "Arrival : " + arrival + "\n" +
			   "Start   : " + start + "\n" +
			   "Finish  : " + finish + "\n" +
			   "Wait    : " + wait + "\n" +
			   "Response: " + response);
    }
    

    /** Show state on a line. For tabular formats. Must figure out how to get table
     *  formatting. Oh, to say %8ld in java.
     */
    public void println(){
	System.out.println("PID " + PID + " b" + burst + " p" +
			   priority +  " a" +
			   arrival + " s" + start +  " f" +
			   finish + " w" + wait +  " r" +
			   response);
			 
    }


    /** Print comma separated values to the terminal
     */
    public void printCSV(){
	System.out.println(PID + "," + initBurst + "," +
			   priority +  "," + arrival + "," + 
			   start +  "," + finish + ","  +
			    wait +  "," + response + "," + lifetime);
			 
    }

    /**
     * Print comma separated values list to a PrintWriter object.
     */
    public void printCSV(PrintWriter pw){
	pw.println(PID + "," + initBurst + "," +
			   priority +  "," + arrival + "," + 
			   start +  "," + finish + ","  +
			    wait +  "," + response + "," + lifetime);
			 
    }


    /**
     * Get the value of response time.
     * @return Value of response time .
     */
    public long getResponseTime() {return response;}
    
    /**
     * Get the value of wait.
     * @return Value of wait.
     */
    public long getWaitTime() {
    	//System.out.println("arrival: " + getArrivalTime() + ", wait: " + wait);
    	return wait;}
    
    
    /**
     * Get the value of finish.
     * @return Value of finish.
     */
    public long getFinishTime() {return finish;}
    
    /**
     * Get the value of start.
     * @return Value of start.
     */
    public long getStartTime() {return start;}
    
    /**
     * Set the value of start.
     * @param v  Value to assign to start.
     */
    //public void setStartTime(long  v) {this.start = v;}
    
    /**
     * Get the value of arrival.
     * @return Value of arrival.
     */
    public long getArrivalTime() {return arrival;}
    
    /**
     * Set the value of arrival.
     * @param v  Value to assign to arrival.
     */
    public void setArrivalTime(long  v) {this.arrival = v;}
    
    /**
     * Get the value of priority.
     * @return Value of priority.
     */
    public long getPriorityWeight() {return priority;}

   /**
     * Get the value of delay.
     * @return Value of delay.
     */
    public long getDelayTime() {return delay;}
    
    /**
     * Get the value of burst.
     * @return Value of burst.
     */
    public long getBurstTime() {return burst;}
    
    /**
     * Get the initial burst value of this process.
     * @return Value of initBurst.
     */
    public long getInitBurstTime() {return initBurst;}
    
    
    /**
     * Get the value of PID.
     * @return Value of PID.
     */
    public long getPID() {return PID;}
    

    /**
     * Get the value of lifetime
     * @return current lifetime in the scheduling queue
     */
    public long getLifetime(){ return lifetime; };


    /** Restores the process to it's original state. For rerunning
	a data set maybe under different circumstances*/
    public void restore(){
	burst = initBurst;
	lifetime = 0;
	response = 0;
	start    = 0;
	wait     = 0;
	active   = false;
	started  = false;
	finished = false;
	arrived  = false;
    }



    /**
     * Get the value of active.
     * @return Value of active.
     */
    public boolean isActive() {return active;}
    

    /**
     * Get the value of finished.
     * @return Value of finished.
     */
    public boolean isFinished() {return finished;}

    
    /**
     * Get the value of started.
     * @return Value of started.
     */
    public boolean isStarted() {return started;}
    

    /**
     * Get the value of arrived.
     * @return Value of arrived.
     */
    public boolean isArrived() { return arrived; }

} // ENDS class Process

 
