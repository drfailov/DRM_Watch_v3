import { Midi } from "@tonejs/midi";
import { Note } from "@tonejs/midi/src/Note";
import { Frequency } from "tone";

const saveFile = (filename: string, data: string) => {
  const blob = new Blob([data], { type: "text/plain" });
  if (window.navigator.msSaveOrOpenBlob) {
    window.navigator.msSaveBlob(blob, filename);
  } else {
    const elem = window.document.createElement("a");
    elem.href = window.URL.createObjectURL(blob);
    elem.download = filename;
    document.body.appendChild(elem);
    elem.click();
    document.body.removeChild(elem);
  }
};

const convertMelody = (notes: Note[]) => {

//sdsdsd

  let lastTime = 0;
  let lastDuration = 0;
  let code = `const int melody[]={`;
  notes.forEach((note) => {
    let freq = Math.round(Frequency(note.name).toFrequency());
	let durationMs = Math.round(note.duration * 1000);
	let timeMs = Math.round(note.time * 1000);
	let prepause = timeMs - (lastTime + lastDuration);
	if (lastTime != 0 && prepause > 15) {
		code += `-${prepause}, `;
	}
	//code += `(${prepause}), `;
    code += `${freq}, -${durationMs}, `;
	lastTime = timeMs;
	lastDuration = durationMs;
  });
  
  code += `19};`;

  return code;
};

export { convertMelody, saveFile };
