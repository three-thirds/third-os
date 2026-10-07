<script lang="ts">
    import { blur, fade } from 'svelte/transition';
    import Leaves from '../lib/Leaves.svelte';
    import Desktop from '../lib/Desktop.svelte';
    import Mist from '../lib/Mist.svelte';
    import Kernal from '../lib/easter_eggs/Kernal.svelte';

    let phase = $state<'intro' | 'desktop'>('intro');
    const enter = () => { phase = 'desktop' }
    let panic = $state(false)
</script>

<svelte:window 
    onclick={() => phase === 'intro' && !panic && enter()} 
    onkeydown={() => phase === 'intro' && !panic && enter()} 
/>

{#if phase === 'intro'}
    <main class="fixed inset-0 grid place-items-center overflow-hidden" out:blur={{ amount: 16, duration: 1100 }}>

        <div class="pointer-events-none absolute inset-0" aria-hidden="true">
        <div class="absolute left-1/4 top-1/4 size-[32rem] rounded-full bg-rust-brown-600/10 blur-3xl animate-drift"></div>
        <div class="absolute bottom-1/4 right-1/4 size-[28rem] rounded-full bg-bronze-500/10 blur-3xl animate-drift [animation-delay:-9s]"></div>
        <div class="absolute left-1/2 top-1/2 size-[24rem] rounded-full bg-dark-garnet-600/15 blur-3xl animate-drift [animation-delay:-16s]"></div>
    </div>

    <Mist text="third.os" class="relative text-7xl md:text-9xl [text-shadow:0_0_24px_rgb(255_255_255/0.35)]" />
    <Leaves onDone={enter} onPanic = {() => (panic = true)} />
    </main>
{:else}
  <div in:fade={{ duration: 1200, delay: 400 }}>
    <Desktop />
  </div>
{/if}

{#if panic}
    <Kernal />
{/if}