<script lang="ts">
    import { onMount } from "svelte"
    import gsap from "gsap"

    let { text, class: cls = ''}: { text: string; class?: string } = $props();

    let el: HTMLElement;
    const chars = $derived([...text])

    onMount(() => {
        const spans = el.querySelectorAll('span');

        if(matchMedia('(prefers-reduced-motion: reduce)').matches) {
            gsap.set(spans, { opacity: 1, filter: 'blur(0px)', fontWeight: 700})
        }

        const tween = gsap.fromTo(
            spans,
            { opacity: 0, filter: 'blur(14px)', fontWeight: 100, y:6, scale: 1.08},
            {
                opacity: 1,
                filter: 'blur(0px)',
                fontWeight: 700,
                y: 0,
                scale: 1,
                duration: 1.8,
                ease: 'power2.out',
                stagger: { each: 0.07, from: 'random' }
            }
        );
        return () => tween.kill();
    });
</script>

<h1 bind:this={el} class="font-doto {cls}" aria-label={text}>
    {#each chars as char}
        <span aria-hidden="true" class="inline-block opacity-0">
        {char === ' ' ? '\u00A0' : char}
        </span>    
    {/each}
</h1>