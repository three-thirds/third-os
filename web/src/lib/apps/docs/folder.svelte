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

