<script lang="ts">
    import type { Folder } from './data'
    let { folders, onOpen} : { folders: Folder[]; onOpen: (f: Folder) => void } = $props();
</script>

{#each folders as folder}
<button onclick={() => onOpen(folder)}>{folder.name}</button>
{/each}