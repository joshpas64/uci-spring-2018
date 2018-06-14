/** SchedulingAlgorithm.java
 * 
 * A scheduling algorithm to be used with Jim Weller's simulator.
 *
 * Adapted for CS 143A - Principles of Operating Systems
 * 
 * @author: Kyle Benson
 * Winter 2013
 *
 */
package com.jimweller.cpuscheduler;

public interface SchedulingAlgorithm {
    /** Add the new job to the correct queue.*/
    public void addJob(Process p);
    
    /** Returns true if the job was present and was removed. */
    public boolean removeJob(Process p);

    /** Returns the next process that should be run by the CPU, null if none available.*/
    public Process getNextJob(long currentTime);

    /** Returns true if the current job is finished or there is no such job. */
    public boolean isJobFinished();

    /** Return a human-readable name for the algorithm. */
    public String getName();

    /** Transfer all the jobs in the queue of a SchedulingAlgorithm to another, such as
	when switching to another algorithm in the GUI */
    public void transferJobsTo(SchedulingAlgorithm otherAlg);
}