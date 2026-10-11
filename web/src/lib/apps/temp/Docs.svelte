<script lang="ts">
    import { marked } from 'marked';
    import readme from './README.md?raw';

    const html = marked.parse(readme) as string;
</script>

<article class="prose prose-neutral dark:prose-invert mx-auto max-w-3xl p-6">
  {@html html}
</article>