<script lang="ts">
    import { apps } from './apps';
    import { wm } from './windows.svelte';
</script>

<nav class="relative z-30 mx-auto mb-4 flex gap-3 rounded-full p-3 border border-bronze-500/30 bg-rich-mahogany-100/90 backdrop-blur-md">
    {#each Object.values(apps).filter((a) => !a.hidden) as app}
        {@const running = wm.wins.some(win => win.appId === app.id)}
        <button
            class="relative grid size-12 place-items-center rounded-xl transition hover:-transpate-y-1 hover:bg-rich-mahogany-300/75 active:bg-rich-mahogany-400/75"
            title={app.title}
            aria-label={app.title}
            onclick={() => wm.launch(app.id)}
        >
            <!-- <span class="size-10 [&>svg]:size-full">{@html app.icon}</span> -->
             <img src={app.icon} alt={app.title} class="size-10 invert" />
             {#if running}
                <span class="absolute -bottom-1.5 size-1 rounded-full bg-bronze-700"></span>
             {/if}
        </button>
    {/each}
</nav>