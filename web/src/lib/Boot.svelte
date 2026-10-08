<script lang="ts">
    import { onMount } from "svelte"
    
    let bootComplete = $state(false);

    const sleep = (ms: number) => new Promise((resolve) => setTimeout(resolve, ms));

    const messages = [
        '[0.000000] Booting ThirdOS kernel 0.1.0...',
        '[0.041284] CPU: Initializing processor...',
        '[0.083912] CPU: Detected x86_64 architecture',
        '[0.127431] CPU: Enabling protected mode',
        '[0.184293] Memory: Initializing physical memory...',
        '[0.241827] Memory: Setting up virtual memory...',
        '[0.318492] Memory: Kernel heap initialized',
        '[0.397201] Kernel: Loading core modules...',
        '[0.461832] Kernel: Initializing interrupt controller',
        '[0.528193] Kernel: Registering system calls',
        '[0.601284] Scheduler: Initializing process scheduler',
        '[0.674921] Scheduler: Task manager online',
        '[0.742183] VFS: Initializing virtual filesystem',
        '[0.819472] VFS: Mounting root filesystem',
        '[0.891203] VFS: / mounted successfully',
        '[0.963821] Driver: Initializing display driver',
        '[1.047291] Driver: Initializing input devices',
        '[1.128473] Driver: Initializing audio subsystem',
        '[1.219384] Network: Initializing network stack',
        '[1.304821] Security: Initializing permission system',
        '[1.391204] Userspace: Starting init process',
        '[1.482193] Userspace: Loading system services',
        '[1.573821] Compositor: Initializing graphics compositor',
        '[1.668294] WindowManager: Starting window manager',
        '[1.761203] Desktop: Loading HNTRO desktop environment',
        '[1.853921] Desktop: Initializing application manager',
        '[1.947283] Desktop: Loading system applications',
        '[2.041832] Desktop: Restoring user session',
        '[2.138492] System: All services initialized',
        '[2.241829] System: Startup complete',
    ];

    let index = 0;
    let timeout: ReturnType<typeof setTimeout>
    let visibleMessages = $state<string[]>([]);

    const showNext = () => {
        if(index <messages.length) {
            visibleMessages = [...visibleMessages, messages[index]];
            index++;
            
            const delay = Math.floor(Math.random() * (1000-167+ 1)) + 167;
            timeout = setTimeout(showNext, delay);
        } else {
            timeout = setTimeout(() => {
                bootComplete = true;
            }, 1423)
        }
    }


    onMount(() => {
        showNext();
        return () => clearTimeout(timeout);
    })
</script>


{#if !bootComplete}
    <main class="fixed inset-0 z-999 bg-black p-8 font-mono text-sm text-white">
        <div class = "max-w-3xl">
            {#each visibleMessages as message}
                <div class="leading-6">{message}</div>
            {/each}

            {#if visibleMessages.length === messages.length}
                <div class="mt-2 text-green-400">
                    Third kernel ready.
                </div>
                <div class="mt-4 animate-pulse">
                    _
                </div>
            {/if}
        </div>
    </main>
{/if}