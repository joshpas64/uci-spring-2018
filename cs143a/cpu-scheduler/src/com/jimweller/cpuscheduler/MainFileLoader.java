/**
 * MainFileLaoder.java
 */
package com.jimweller.cpuscheduler;

import java.util.*;

/**
 * @author Charles Zhu
 * @version 1.0 Spring 2016
 */
public class MainFileLoader {
	
	private static HashMap<String, Class<? extends SchedulingAlgorithm>> algs;
	
	static {
		algs = new HashMap<String, Class<? extends SchedulingAlgorithm>>();
		algs.put("Random", RandomSchedulingAlgorithm.class);
		algs.put("FCFS", FCFSSchedulingAlgorithm.class);
		algs.put("SJF", SJFSchedulingAlgorithm.class);
		algs.put("Priority", PrioritySchedulingAlgorithm.class);
		algs.put("RR", RoundRobinSchedulingAlgorithm.class);
	}

	public static void main(String[] args) {
		if(args.length < 2) {
			printUsage();
			System.exit(-1);
		}
		
		String configFileName = args[0];
		String algName = args[1];
		CPUScheduler cpu;
		SchedulingAlgorithm alg = new RandomSchedulingAlgorithm();
//		SchedulingAlgorithm alg;
		
		int quantumRR;
		@SuppressWarnings("unused")
		boolean preemptive = false;
		
		if(!algs.containsKey(algName)){
			System.err.println("Could not resolve the specified algorithm.");
			System.err.println();
			printAlgorithms();
			System.exit(-3);
		}
		try {
			alg = algs.get(algName).newInstance();
		} catch (InstantiationException | IllegalAccessException ex) {
			// TODO Auto-generated catch block
			ex.printStackTrace();
			System.exit(-4);
		}
		
		// Parse optional parameters for algorithms
		// Simple algorithms cannot have more than two arguments
		if(!(alg instanceof OptionallyPreemptiveSchedulingAlgorithm)){
			if(!(alg instanceof RoundRobinSchedulingAlgorithm)) {
				if(args.length > 2) {
					System.err.println("Too many arguments.");
					System.err.println();
					printAlgorithms();
					System.exit(-5);
				}
			} else {
				if(args.length > 3) {
					System.err.println("Too many arguments.");
					System.err.println();
					printAlgorithms();
					System.exit(-5);
				} else if(args.length > 2) {
					try {
						quantumRR = Integer.parseInt(args[2]);
						if(quantumRR <= 0) {
							throw new NumberFormatException();
						}
						((RoundRobinSchedulingAlgorithm) alg).setQuantum(quantumRR);
					} catch(NumberFormatException ex) {
						System.err.println("Invalid QUANTUM.");
						System.exit(-6);
					}
				}
			}
		} else { // Optionally preemptive algorithms
			if(args.length > 3) {
				System.err.println("Too many arguments.");
				System.err.println();
				printAlgorithms();
				System.exit(-5);
			} else if(args.length > 2) {
				if(args[2].toUpperCase().compareTo("PREEMPT") == 0
						|| args[2].toUpperCase().compareTo("PREEMPTIVE") == 0
						|| args[2].toUpperCase().compareTo("YES") == 0
						|| args[2].toUpperCase().compareTo("Y") == 0
						|| args[2].toUpperCase().compareTo("TRUE") == 0
						|| args[2].toUpperCase().compareTo("T") == 0
						|| args[2].toUpperCase().compareTo("1") == 0) {
					((OptionallyPreemptiveSchedulingAlgorithm) alg).setPreemptive(true);
				} else if(args[2].toUpperCase().compareTo("NON") == 0
						|| args[2].toUpperCase().compareTo("NO") == 0
						|| args[2].toUpperCase().compareTo("N") == 0
						|| args[2].toUpperCase().compareTo("FALSE") == 0
						|| args[2].toUpperCase().compareTo("F") == 0
						|| args[2].toUpperCase().compareTo("0") == 0) {
					((OptionallyPreemptiveSchedulingAlgorithm) alg).setPreemptive(false);
				} else {
					System.err.println("Invalid PREEMPT.");
					System.exit(-6);
				}
			}
		}
		
		// Parse JOBS configuration file
		cpu = new CPUScheduler(configFileName);
		System.err.println("Read " + cpu.getJobs().size() + " job(s).");
		if(cpu.getJobs().size() < 1) {
			System.err.println("Could not read jobs from the specified config file.");
			System.exit(-2);
		}
		cpu.setAlgorithm(alg);
		System.err.println("Selected algorithm: " + alg.getName());
		
		// Run the selected algorithm using the loaded jobs
		while(cpu.nextCycle()) {
			;
		}
		System.err.println("Simulation complete.");
		System.err.println();
		
		// Print all jobs
		cpu.printCSV();
	}
	
	private static void printUsage() {
		System.err.println("Usage: java -jar CPUSchedulingSimulator.jar"
				+ " JOBS ALGORITHM");
		System.err.println("       java -jar CPUSchedulingSimulator.jar"
				+ " JOBS ALGORITHM [PREEMPT]");
		System.err.println("       java -jar CPUSchedulingSimulator.jar"
				+ " JOBS RR [QUANTUM]");
		System.err.println("Run the list of jobs using the specified algortihm.");
		System.err.println();
		printAlgorithms();
		System.err.println();
		System.err.println("QUANTUM is an option if ALGORITHM is RR (Round Robin).");
		System.err.println("The default QUANTUM is 10.");
	}

	private static void printAlgorithms() {
		System.err.println("Supported algorithms are:");
		for(Map.Entry<String, Class<? extends SchedulingAlgorithm> > entry: 
				algs.entrySet()) {
			try {
				SchedulingAlgorithm alg = entry.getValue().newInstance();
				System.err.printf("    %-16s    %s",
						entry.getKey(), alg.getName());
				if(alg instanceof OptionallyPreemptiveSchedulingAlgorithm) {
					System.err.printf(" (Optionally Preemptive)");
				}
				System.err.println();
			} catch (InstantiationException | IllegalAccessException ex) {
				// TODO Auto-generated catch block
				ex.printStackTrace();
			}
		}
	}
	
}
