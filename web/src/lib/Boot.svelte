<script lang="ts">
    import { onMount } from "svelte"
    import { fade } from 'svelte/transition';
    
    let bootComplete = false;

    const messages = [
        '[0.000000] Booting ThirdOS...',
    ]

    let visibleMessages: string[] = [];

    onMount(() => {
        let index=0;

        const interval = setInterval(() => {
            if (index < messages.length) {
                visibleMessages = [...visibleMessages, messages[index]];
                index++;
            } else {
                clearInterval(interval);
                bootComplete = true;
            }
        }, 180);

        return () => clearInterval(interval);
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
                    -
                </div>
            {/if}
        </div>
    </main>
{/if}