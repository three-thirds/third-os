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

<App />