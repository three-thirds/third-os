<script lang="ts">
    import { onMount } from 'svelte';

    let { text, class: cls=''}: { text: string, class?: string } = $props();

    const chars = $derived([...text]);
    let delays = $state<number[]>([]);

    onMount(() => {
        delays = chars.map(() => Math.random() * 700);
    })
</script>

<h1 class=" font-doto {cls}" aria-label={text}>
    {#if delays.length}
        {#each chars as c,i}
            <span aria-hidden="true" class="inline-block animate-mist motion-reduce:animate-none" style="animation-delay:{delays[i]}ms">
                {c  === ' ' ? '\u00A0' : c}
            </span>
        {/each}
    {/if}
</h1>