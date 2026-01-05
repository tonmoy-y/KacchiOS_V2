#!/usr/bin/env python3
"""
KacchiOS Scheduler Simulator
এই script KacchiOS এর round-robin scheduler কিভাবে কাজ করে সেটা simulate করে।
"""

class Process:
    """Process Control Block (PCB) representation"""
    def __init__(self, pid):
        self.pid = pid
        self.state = "READY"  # READY, RUNNING, TERMINATED
        self.runtime = 0
    
    def __repr__(self):
        return f"PID={self.pid} STATE={self.state} RUNTIME={self.runtime}"

class Scheduler:
    """Round-Robin Scheduler"""
    def __init__(self, max_processes=8):
        self.processes = []
        self.current_idx = -1
        self.max_processes = max_processes
        self.tick_count = 0
        self.quantum = 3  # Time quantum (simulated)
    
    def create_process(self, pid):
        """Create a new process"""
        if len(self.processes) >= self.max_processes:
            print(f"❌ Cannot create process {pid}: Maximum {self.max_processes} reached")
            return False
        
        p = Process(pid)
        self.processes.append(p)
        print(f"✅ Process {pid} created (State: {p.state})")
        return True
    
    def schedule(self):
        """Round-robin scheduling"""
        if not self.processes:
            return None
        
        # Mark current process as READY (if running)
        if self.current_idx >= 0:
            if self.processes[self.current_idx].state == "RUNNING":
                self.processes[self.current_idx].state = "READY"
        
        # Find next READY process
        for i in range(len(self.processes)):
            next_idx = (self.current_idx + i + 1) % len(self.processes)
            p = self.processes[next_idx]
            
            if p.state == "READY":
                p.state = "RUNNING"
                self.current_idx = next_idx
                return p
        
        return None
    
    def run_tick(self):
        """Simulate one scheduling tick"""
        self.tick_count += 1
        
        # Run current process
        if self.current_idx >= 0:
            current = self.processes[self.current_idx]
            if current.state == "RUNNING":
                current.runtime += 1
        
        # Context switch after quantum expires
        if self.tick_count % self.quantum == 0:
            p = self.schedule()
            if p:
                print(f"🔄 Tick {self.tick_count}: Switched to Process {p.pid}")
    
    def show_status(self):
        """Display process status (like 'ps' command)"""
        print("\n" + "="*40)
        print("PID  STATE       RUNTIME")
        print("-"*40)
        for p in self.processes:
            marker = "👉" if p.state == "RUNNING" else "  "
            print(f"{marker} {p.pid}    {p.state:10s}  {p.runtime}")
        print("="*40 + "\n")
    
    def terminate_all(self):
        """Terminate all processes"""
        for p in self.processes:
            p.state = "TERMINATED"
        print("⛔ All processes terminated")

def main():
    """Simulate KacchiOS scheduling behavior"""
    print("🍚 KacchiOS Scheduler Simulator")
    print("="*40)
    
    scheduler = Scheduler(max_processes=8)
    
    # Scenario 1: Create processes (like 'demo start')
    print("\n📌 Scenario 1: Creating Processes")
    print("-"*40)
    scheduler.create_process(1)
    scheduler.create_process(2)
    
    # Show initial status
    scheduler.show_status()
    
    # Scenario 2: Run scheduler for several ticks
    print("📌 Scenario 2: Running Scheduler")
    print("-"*40)
    
    # First schedule
    p = scheduler.schedule()
    print(f"▶️  Initial schedule: Process {p.pid} is RUNNING\n")
    scheduler.show_status()
    
    # Simulate multiple scheduling ticks
    for i in range(10):
        scheduler.run_tick()
        
        # Show status every 3 ticks (after context switch)
        if (i + 1) % 3 == 0:
            scheduler.show_status()
    
    # Scenario 3: Terminate processes (like 'demo stop')
    print("📌 Scenario 3: Terminating Processes")
    print("-"*40)
    scheduler.terminate_all()
    scheduler.show_status()
    
    # Scenario 4: Create more processes
    print("📌 Scenario 4: Creating More Processes (PIDs continue)")
    print("-"*40)
    scheduler.processes = []  # Reset process table
    scheduler.create_process(3)
    scheduler.create_process(4)
    scheduler.create_process(5)
    
    p = scheduler.schedule()
    print(f"\n▶️  Scheduled: Process {p.pid} is RUNNING")
    scheduler.show_status()
    
    # Run a few ticks
    for i in range(6):
        scheduler.run_tick()
    
    scheduler.show_status()
    
    print("\n✨ Simulation Complete!")
    print("\n💡 Key Points:")
    print("   • Round-robin: Each process gets equal CPU time")
    print("   • Context switch: Happens every quantum (3 ticks)")
    print("   • States: READY → RUNNING → READY (circular)")
    print("   • PIDs: Increment continuously (1, 2, 3, 4, ...)")

if __name__ == "__main__":
    main()
