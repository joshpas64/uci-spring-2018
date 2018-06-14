/** RoundRobinSchedulingAlgorithm.java
 * 
 * A scheduling algorithm that randomly picks the next job to go.
 *
 * @author: Kyle Benson
 * Winter 2013
 *
 */
package com.jimweller.cpuscheduler;

import java.util.*;

public class RoundRobinSchedulingAlgorithm extends BaseSchedulingAlgorithm {
    private static final int DEFAULT_QUANTUM = 20;

    //Create a comparator so the Unfinished Process queue can be sorted by Process ID
    private class PIDComparator implements Comparator<Process> {
        public int compare(Process p1,Process p2){
            return (int) (p1.getPID() - p2.getPID()); //Compare by Process ID
        }
    }

    /** the time slice each process gets */
    private int quantum;
    private LinkedList<Process> unfinishedJobs;
    private int currentTick;
    private PIDComparator comparator;

    RoundRobinSchedulingAlgorithm() {
        // Fill in this method
        /*------------------------------------------------------------*/
        this.activeJob = null;
        unfinishedJobs = new LinkedList<Process>();
        currentTick = 0;
        quantum = DEFAULT_QUANTUM; //Have quantum set to 20 by default
        /*------------------------------------------------------------*/
    }

    /** Add the new job to the correct queue. */
    public void addJob(Process p) {
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/

        //Add the job to the unfinished process queue if it is not already present
        if(!this.unfinishedJobs.contains(p)){
            this.unfinishedJobs.add(p);
        }
        /*------------------------------------------------------------*/
    }

    /** Returns true if the job was present and was removed. */
    public boolean removeJob(Process p) {
        // Remove the next lines to start your implementation

        //Remove process from the queue of unifinished jobs that are cycled back and forth
        //from the CPU
        return this.unfinishedJobs.remove(p);
        /*------------------------------------------------------------*/
    }

    /** Transfer all the jobs in the queue of a SchedulingAlgorithm to another, such as
    when switching to another algorithm in the GUI */
    public void transferJobsTo(SchedulingAlgorithm otherAlg) {
        throw new UnsupportedOperationException();
    }

    /**
     * Get the value of quantum.
     * 
     * @return Value of quantum.
     */
    public int getQuantum() {
        return quantum;
    }

    /**
     * Set the value of quantum.
     * 
     * @param v
     *            Value to assign to quantum.
     */
    public void setQuantum(int v) {
        this.quantum = v;
    }

    //Private Method for determining which process to pick
    private Process findClosestProcess(){

        //Ensure that the process queue is sorted by process ID for traversal.
        Collections.sort(this.unfinishedJobs,new PIDComparator());

        //If there was no process currently executing on the CPU,
        //the next process is simply the process queue's first item.
        if(this.activeJob == null){
            if(this.unfinishedJobs.size() > 0){
                return this.unfinishedJobs.get(0);
            }            
        } else {

            //Find the first process with a higher process ID than the currently executing
            //process and set it as the new process to execute next.
            for(int i = 0; i < this.unfinishedJobs.size(); i++){
                Process temp = this.unfinishedJobs.get(i);
                if(temp.getPID() > this.activeJob.getPID()){
                    return temp;
                }
            }

            //If there are no processes with a higher process ID than the current process, use the 
            //process queue's first item as a default.
            if(this.unfinishedJobs.size() > 0){
                return this.unfinishedJobs.get(0);
            }
        }
        return null;
    }




    /**
     * Returns the next process that should be run by the CPU, null if none
     * available.
     */

    public Process getNextJob(long currentTime) {
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/

        //Iterate the timer if there is no active job in the queue or if the currently executing job (process) has not yet finished
        if(this.activeJob != null && !(this.activeJob.isFinished())){
            currentTick++; //Incremente time

            //A process's time slice is up when its timer becomes greater than the algorithm's quantum value.
            if(currentTick >= quantum){
                currentTick = 0; //Reset timer for newly executing process to 0.
                if(unfinishedJobs.size() > 0){
                    Process tail = this.activeJob;
                    this.activeJob = findClosestProcess(); //Determine next process to execute from the process queue.

                    //Move the current process back to the unfinished process queue if it is not already present
                    if(this.activeJob != null && !(this.unfinishedJobs.contains(this.activeJob)))
                        unfinishedJobs.add(tail);
                }
            }
        }

        //In the case that there is no process currently executing or if the current process has already finished its
        //burst, determine the next process to execute from the process queue and begin executing it (setting it as the active job)
        if(unfinishedJobs.size() > 0 && (this.activeJob == null || this.activeJob.isFinished())){
            currentTick = 0;
            this.activeJob = findClosestProcess();
        }

        return this.activeJob;

        /*------------------------------------------------------------*/
    }

    public String getName() {
        return "Round Robin";
    }
    
}