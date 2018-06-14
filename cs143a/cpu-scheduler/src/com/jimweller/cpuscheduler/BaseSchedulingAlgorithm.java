/** BaseSchedulingAlgorithm.java
 * 
 * An abstract scheduling algorithm for others to inherit from.
 *
 * @author: Kyle Benson
 * Winter 2013
 *
 */
package com.jimweller.cpuscheduler;

public abstract class BaseSchedulingAlgorithm implements SchedulingAlgorithm {
    /** The currently running process, null if none. */
    protected Process activeJob;

    /** Add the new job to the correct queue.*/
    public abstract void addJob(Process p);
    
    /** Returns true if the job was present and was removed. */
    public abstract boolean removeJob(Process p);

    /** Transfer all the jobs in the queue of a SchedulingAlgorithm to another, such as
	when switching to another algorithm in the GUI */
    public abstract void transferJobsTo(SchedulingAlgorithm otherAlg);

    /** Returns the next process that should be run by the CPU, null if none available.*/
    public abstract Process getNextJob(long currentTime);

    /** Return a human-readable name for the algorithm. */
    public abstract String getName();

    /** Returns true if the current job is finished or there is no such job. */
    public boolean isJobFinished(){
	if (activeJob != null)
	    return activeJob.isFinished();
	else
	    return true;
    }
}
