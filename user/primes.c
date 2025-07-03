#include "kernel/types.h"
#include "user.h"

int main() {
	int p[2];
	pipe(p);

	int pid = fork();
	if (pid < 0) {
		fprintf(2, "fork failed\n");
		exit(1);
	}

	if (pid == 0) {
		// ==== Child Process ====

		// Set pipe ends
		close(p[1]);
		int left = p[0];

		while (1) {
			// ==== Child Process Work Template ====
			// 0. Done: left pipe setup
			// 1. Read the first number and print
			// 2. Create right pipe
			// 3. Fork the right child
			// 4. 4.1 for its child, 4.2 for itself: 
			// 4.1.1 Child: set left pipe read end (Work preparation)
			// 4.1.2 Jump to the starting point of template (Starting work)
			// 4.2.1 Current: set right pipe write end
			// 4.2.1 Current: set right pipe write end
			// 4.2.3 Clean up and wait
			// 4.2.4 Work done

			// 1. Print the first number from pipe
			int prime;
			if (read(left, &prime, sizeof(int)) != sizeof(int)) {
				fprintf(2, "read error\n");
				exit(1);
			}
			printf("prime %d\n", prime);

			// 2. Create right pipe
			int right_pipe[2];
			pipe(right_pipe);

			// 3. Fork the right child
			pid = fork();
			if (pid < 0) {
				fprintf(2, "fork failed\n");
				exit(1);
			}

			// 4. 4.1 for its child, 4.2 for itself: 
			if (pid == 0) {
				// ==== Right child ====
				// 4.1.1 Child: set left pipe read end (Work preparation)
				close(right_pipe[1]);
				left = right_pipe[0];

				// 4.1.2 Jump to the starting point of template (Starting work)
				continue; 
			} else {
				// 4.2.1 Current: set right pipe write end
				close(right_pipe[0]);
				int n;
				// 4.2.1 Current: set right pipe write end
				while (read(left, &n, sizeof(int)) == sizeof(int)) {
					if (n % prime != 0)
						write(right_pipe[1], &n, sizeof(int));
				}
			}

			// 4.2.3 Clean up and wait
			close(right_pipe[1]);
			close(left);
			wait(0);

			// 4.2.4 Work done
			break; 
		}
	} else {
		// ==== Parent Process ====
		close(p[0]);
		for (int i = 2; i <= 280; i++) {
			if (write(p[1], &i, sizeof(int)) != sizeof(int)) {
				fprintf(2, "write error\n");
				exit(1);
			}
		}

		close(p[1]);
		wait(0);
	}

	exit(0);
}