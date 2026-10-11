export type FileItem = { name: string; content: string };
export type Folder = { id: string; name: string; files?: FileItem[] };

const md = import.meta.glob('../../../../../docs/*.md', {
    query: '?raw',
    import: 'default',
    eager: true
}) as Record<string, string>;

const documentFIles: FileItem[] =Object.entries(md)
    .map(([path, content]) => ({ name: path.split('/').pop() || '', content }))
    .sort((a,b) => a.name.localeCompare(b.name));


export const folders: Folder[] = [
  {
    id: 'music',
    name: 'Music',
    files: [
      { name: 'song1.mp3', content: 'This is song 1' },
      { name: 'song2.mp3', content: 'This is song 2' }
    ]
  }
];