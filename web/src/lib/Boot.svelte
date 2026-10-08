<script lang="ts">
    import { onMount } from "svelte"
    import messages from '../lib/assets/boot.json'
    
    let bootComplete = $state(false);

    const sleep = (ms: number) => new Promise((resolve) => setTimeout(resolve, ms));


    let index = 0;
    let timeout: ReturnType<typeof setTimeout>
    let visibleMessages = $state<string[]>([]);

    const showNext = () => {
        if(index <messages.length) {
            visibleMessages = [...visibleMessages, messages[index]];
            index++;
            
            const delay = Math.floor(Math.random() * (50-7+ 1)) + 7;
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