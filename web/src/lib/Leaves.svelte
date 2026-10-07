<script lang="ts">
    import { onMount } from "svelte"
    import gsap from 'gsap'

    let { count = 70, onDone, onPanic}: { count?: number; onDone?: () => void; onPanic?: () => void} = $props();

    let box: HTMLElement;
    //mabye acually make the colors good future me idk
    const colors = ['#d58936', '#d58936', '#a44200', '#e95d00', '#ac2118', '#e33c30', '#dd9f5d'];

    onMount(() => {
        const r = gsap.utils.random;
        const H = innerHeight;

        const ctx = gsap.context(() => {
            const tl = gsap.timeline({ onComplete: () => onDone?.() });

            gsap.utils.toArray<HTMLElement>('.leaf').forEach((leaf) => {
                const inner = leaf.querySelector('.inner') as HTMLElement;
                //size thing the original was 14, 40 for @Willgob to remember. 
                const size = r(24, 50);
                const far = size < 22;
                const duration = far ? r(6,9): r(4,7);
                const start = r(0,5)

                gsap.set(leaf, {
                    left: `${r(-5,100)}%`,
                    top: 0,
                    y: -80,
                    width: size,
                    height: size,
                    color: gsap.utils.random(colors),
                    opacity: far ? r(0.2,0.5) : r(0.5,1),
                    filter: far ? 'blur(1.5px)' : 'none'
                })
                gsap.set(inner, { transformPerspective: 500})

                tl.to(leaf, { y: H + 80, duration: duration, ease: 'none'}, start);

                const dir = Math.random() < 0.5 ? -1 : 1;
                tl.to(inner,
                    {
                        x: r(50, 130) * dir,
                        rotation: r(-100, 100),
                        rotationX: r(-70, 70),
                        rotationY: r(-70, 70),
                        duration: duration/4,
                        ease: 'sine.inOut',
                        yoyo: true,
                        repeat: 3,
                    },
                    start
                )
            })
        }, box)

        return () => ctx.revert();
    })
</script>



<div bind:this={box} class="pointer-events-none fixed inset-0 overflow-hidden" aria-hidden="true">
  {#each { length: count } as _}
    <!-- svelte-ignore a11y_click_events_have_key_events, a11y_no_static_element_interactions -->
    <div class="leaf absolute top-0 opacity-0 pointer-events-auto p-2" onclick={(e) => { console.log('bleh panic now'); e.stopPropagation(); onPanic?.() }}>
      <svg class="inner size-full" viewBox="0 0 24 24" fill="currentColor">
        <path d="M12 2C6 6 4 12 6 18c2 3 5 4 6 4s4-1 6-4c2-6 0-12-6-16z" />
        <path d="M12 22V8" stroke="#0c0405" stroke-opacity=".35" stroke-width=".8" fill="none" />
      </svg>
    </div>
  {/each}
</div>