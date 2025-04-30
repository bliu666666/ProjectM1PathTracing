import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16]
times = [1564.57, 1182.46, 562.388, 281.532, 206.144]

# Ideal time: single thread time divided by number of threads
ideal_times = [times[0] / t for t in threads]

# Speedup ratio: single thread time divided by current thread time
speedup = [times[0] / t for t in times]

# --- First graph: Time vs number of threads ---
plt.figure(figsize=(8,6))
plt.plot(threads, times, marker='o', label='Actual Time')
plt.plot(threads, ideal_times, linestyle='--', marker='x', label='Ideal Time')
plt.xlabel('Number of Threads')
plt.ylabel('Time (seconds)')
plt.title('Strong Scaling: Time vs Threads')
plt.xticks(threads)
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend()
plt.tight_layout()
plt.savefig('time_vs_threads_linear.png')
plt.show()

# --- Second graph: Speedup vs number of threads ---
plt.figure(figsize=(8,6))
plt.plot(threads, speedup, marker='s', label='Measured Speedup')
plt.plot(threads, threads, linestyle='--', marker='o', label='Ideal Speedup')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('Strong Scaling: Speedup vs Threads')
plt.xticks(threads)
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend()
plt.tight_layout()
plt.savefig('speedup_vs_threads_linear.png')
plt.show()