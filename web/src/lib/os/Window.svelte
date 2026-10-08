<script lang="ts">
    import { scale } from 'svelte/transition';
    import { wm, type WindowState } from './windows.svelte';
    import { apps } from './apps';

    let { win }: {win: WindowState} = $props();
    const App = $derived(apps[win.appId].component);

    let drag: { dx: number; dy: number} | null = null;

    function down(e: PointerEvent) {
        if (win.maximized) return;
        drag = { dx: e.clientX - win.x, dy: e.clientY - win.y};
        (e.currentTarget as HTMLElement).setPointerCapture(e.pointerId);
    }

    function move(e: PointerEvent) {
        if (!drag) return;
        win.x = Math.max(0, Math.min(innerWidth - 120, e.clientX - drag.dx))
        win.y = Math.max(32, Math.min(innerHeight - 48, e.clientY - drag.dy))
    }

    const up = () => (drag = null);
</script>

<!-- svelte-ignore a11y_no_static_element_interactions -->
<section
    in:scale = {{ start: 0.94, duration: 167}}
    class="pointer-events-auto absolute flex flex-col overflow-hidden rounded-xl border border-bronze-700/20 bg-rich-mahogany-100/92 shadow-lg backdrop-blur-md"
    style:display={win.minimized ? 'none' : undefined}
    style:left={win.maximized ? '0' : `${win.x}px`}
    style:top={win.maximized ? '0' : `${win.y}px`}
    style:width={win.maximized ? '0' : `${win.w}px`}
    style:height={win.maximized ? '0' : `${win.h}px`}
    style:right={win.maximized ? '0' : undefined}
    style:bottom={win.maximized ? '6rem' : undefined}
    style:z-index={win.z}
    onpointerdown={() => wm.focus(win.id)}
>

    <!-- this thing is da 3 buttons up top :thumb-up: -->
    <div
        class="flex cursor-grab touch-none select-none items-center gap-2 border-b border-bronze-500/20 py-2 active:cursor-grabbing"
        onpointerdown={down}
        onpointermove={move}
        onpointerup={up}
        onpointercancel={up}
        ondblclick={() => (win.maximized = !win.maximized)}
    >
        <button class="size-3 rounded-full bg-dark-garnet-700" aria-label="Close" onpointerdown={(e) => e.stopPropagation()} onclick={() => wm.close(win.id)}></button>
        <button class="size-3 rounded-full bg-dark-garnet-700" aria-label="Close" onpointerdown={(e) => e.stopPropagation()} onclick={() => wm.close(win.id)}></button>
        <button class="size-3 rounded-full bg-dark-garnet-700" aria-label="Close" onpointerdown={(e) => e.stopPropagation()} onclick={() => wm.close(win.id)}></button>
    </div>


    <div class="min-h-0 flex-1 overflow-auto text-bronze-800">
    <App />
    </div>
    
</section>


