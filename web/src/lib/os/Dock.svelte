<script lang="ts">
    import { apps } from './apps';
    import { wm } from './windows.svelte';
</script>

<nav class="relative z-30 mx-auto mb-4 flex gap-3 rounded-2xl border border-bronze-600 p-3 backdrop-blur">
    {#each Object.values(apps) as app}
        {@const running = wm.wins.some(win => win.appId === app.id)}
        <button
            class="relative grid size-12 place-items-center rounded-xl"
            title={app.title}
            aria-label={app.title}
            onclick={() => wm.launch(app.id)}
        >
            {app.icon}
        </button>
    {/each}
</nav>