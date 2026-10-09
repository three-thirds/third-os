<script lang="ts">
    import type { Folder, FileItem } from './data'

    let {
        folder,
        file,
        onFile,
        onBack
    } : { folder: Folder; file: FileItem | null; onFile: (f: FileItem) => void; onBack: () => void } = $props();
</script>

<button onclick={onBack}>backe</button>
{folder.name}

{#if file} 
    <pre>{file.content}</pre>
{:else}
    {#each folder.files ?? [] as f}
        <button onclick={() => onFile(f)}>{f.name}</button>
    {/each}
{/if}

