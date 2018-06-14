/** FCFSSchedulingAlgorithm.java
 * 
 * A first-come first-served scheduling algorithm.
 *
 * @author: Charles Zhu
 * Spring 2016
 *
 */
package com.jimweller.cpuscheduler;

import java.util.*;

public class FCFSSchedulingAlgorithm extends BaseSchedulingAlgorithm {
    private LinkedList<Process> processQueue;


    FCFSSchedulingAlgorithm(){
        // Fill in this method
        /*------------------------------------------------------------*/
        this.activeJob = null;
        this.processQueue = new LinkedList<Process>(); //Have collection of Processing be represented as a simple LinkedList Queue in Java.
        /*------------------------------------------------------------*/
    }

    /** Add the new job to the correct queue.*/
    public void addJob(Process p){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/
        //Add new process to the tail of the queue of processes
        this.processQueue.add(p);


        /*------------------------------------------------------------*/
    }
    
    /** Returns true if the job was present and was removed. */
    public boolean removeJob(Process p){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();

        // Fill in this method
        /*------------------------------------------------------------*/
        //Find process in waiting queue and remove it.
        return this.processQueue.remove(p);  


        /*------------------------------------------------------------*/
    }

    /** Transfer all the jobs in the queue of a SchedulingAlgorithm to another, such as
    when switching to another algorithm in the GUI */
    public void transferJobsTo(SchedulingAlgorithm otherAlg) {
        throw new UnsupportedOperationException();
    }

    /** Returns the next process that should be run by the CPU, null if none available.*/
    public Process getNextJob(long currentTime){
        // Remove the next lines to start your implementation
        //throw new UnsupportedOperationException();
        
        // Fill in this method
        /*------------------------------------------------------------*/
        //Set the next process to execute to the front item in the waiting queue of processors.
        //whenever there is no process currently executing or the currently executing process finishes its burst.
        if(this.processQueue.size() > 0 && (this.activeJob == null || this.activeJob.isFinished())){
            this.activeJob = this.processQueue.remove();
        }
        return this.activeJob;
        /*------------------------------------------------------------*/
    }

    public String getName(){
        return "First-Come First-Served";
    }
    
}