export type FileItem = { name: string; content: string };
export type Folder = { id: string; name: string; files?: FileItem[] };

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